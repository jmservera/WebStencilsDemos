//---------------------------------------------------------------------------
#include "MainWebModule.h"
#include <IdHTTPWebBrokerBridge.hpp>
#include <IdContext.hpp>
#include <System.IOUtils.hpp> // Include for TPath
#include <System.DateUtils.hpp> // Include for FormatDateTime
#include <vector> // Include for std::vector used in DefineRoutes

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"

TComponentClass WebModuleClass = __classid(TMainWebModule);
//---------------------------------------------------------------------------

__fastcall TMainWebModule::TMainWebModule(TComponent* Owner)
	: TWebModule(Owner)
{
	// Assign event handler AFTER components are created
	WebStencilsEngine->OnValue = WebStencilsEngineValue;

	InitRequiredData();
    InitControllers();
	DefineRoutes();
}
//---------------------------------------------------------------------------

__fastcall TMainWebModule::~TMainWebModule()
{
	// Ensure components are disconnected/inactive before controllers are destroyed
    if (Customers && Customers->Active)
	{
		Customers->Active = false;
	}
	// Controllers are destroyed automatically by unique_ptr
	// FCustomersController.reset(); // Implicitly called by unique_ptr destructor
	// FCodeExamples.reset();
	// FTasksController.reset();
}
//----------------------------------------------------------------------------
void TMainWebModule::WebStencilsEngineValue(TObject* Sender, const String AObjectName,
			const String APropName, String &AReplaceText, bool &AHandled)
{
	if (SameText(AObjectName, "year"))
	{
		AReplaceText = FormatDateTime("yyyy", Now());
		AHandled = true;
	}
	// Add more handlers here if needed
}
//---------------------------------------------------------------------------

void TMainWebModule::InitControllers()
{
    FTasksController = std::make_unique<TTasksController>(WebStencilsEngine);
	FCustomersController = std::make_unique<TCustomersController>(WebStencilsEngine, Customers);
    // Initialize FCodeExamples here as well, assuming it was missed before
	FCodeExamples = std::make_unique<TCodeExamples>(WebStencilsEngine);
}

void TMainWebModule::InitRequiredData()
{
    try {
		// Delphi uses ParamStr(0), C++Builder typically uses Application->ExeName or similar
		// For simplicity, we'll keep the relative path logic for now, assuming execution from a standard output dir
		#ifdef _WIN32
			FResourcesPath = System::Ioutils::TPath::Combine(System::Ioutils::TPath::GetDirectoryName(GetModuleName(0)), "../../../../resources");
		#else // Assuming Linux or other non-Windows
			FResourcesPath = System::Ioutils::TPath::GetDirectoryName(GetModuleName(0));
		#endif

		// Normalize the path to handle potential .. variations
		FResourcesPath = System::Ioutils::TPath::GetFullPath(FResourcesPath);

        WebStencilsEngine->RootDirectory = System::Ioutils::TPath::Combine(FResourcesPath, "html");
        WebFileDispatcher->RootDirectory = WebStencilsEngine->RootDirectory;

		// --- Replace JSON loading with DB Path setting ---
		String dbPath = System::Ioutils::TPath::Combine(FResourcesPath, "data/database.sqlite3");

		// Ensure the Connection component is assigned and configured in the DFM
		if (Connection)
		{
			Connection->Params->Database = dbPath;
			// Optionally, ensure connection is active or test it
			// try { Connection->Connected = true; } catch(...) { /* Handle connection error */ }
		}
		else
		{
			printf("Warning: TFDConnection component 'Connection' is not assigned.\n");
		}

		// The Customers TFDQuery should be linked to the Connection in the DFM.
		// We no longer load from JSON. Activate the query if needed (or let controllers handle it).
		// if (Customers) { Customers->Active = true; } // Optional: Activate here if needed globally

		// Remove the old AddVar for the 'customers' TFDMemTable/TFDQuery.
		// Controllers will handle providing data to templates.
        WebStencilsEngine->AddVar("customers", Customers, false); // REMOVED
		// --- End of DB Path Setting ---

    }
    catch (Exception& E) {
        printf("Error in InitRequiredData: %s\n", AnsiString(E.Message).c_str());
        throw; // Re-throw the exception
    }
}

void TMainWebModule::DefineRoutes()
{
    std::vector<TRoute> routes = {
        // Tasks Routes (existing)
        TRoute(mtDelete, "/tasks", FTasksController->DeleteTask),
        TRoute(mtPost, "/tasks/add", FTasksController->CreateTask),
        TRoute(mtGet, "/tasks/edit", FTasksController->GetEditTask),
        TRoute(mtPut, "/tasks/toggleCompleted", FTasksController->ToggleCompletedTask),
        TRoute(mtPut, "/tasks", FTasksController->EditTask),
        // Customers Routes (new)
		TRoute(mtGet, "/bigtable", FCustomersController->GetAllCustomers),
		TRoute(mtGet, "/pagination", FCustomersController->GetCustomers)
    };

    // Ensure ClassHelpers.h defines TWebModuleHelper::AddRoutes correctly
    TWebModuleHelper::AddRoutes(this, routes);
}

//---------------------------------------------------------------------------



