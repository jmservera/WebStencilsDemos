//---------------------------------------------------------------------------
#include "MainWebModule.h"
#include <IdHTTPWebBrokerBridge.hpp>
#include <IdContext.hpp>
#include <System.IOUtils.hpp> // Include for TPath
#include <System.DateUtils.hpp> // Include for FormatDateTime
#include <System.SysUtils.hpp> // Include for SameText and UpperCase
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
}
//----------------------------------------------------------------------------
void TMainWebModule::WebStencilsEngineValue(TObject* Sender, const String AObjectName,
			const String APropName, String &AReplaceText, bool &AHandled)
{
	// Check if we're accessing environment variables
	if (SameText(AObjectName, "env"))
	{
		String envValue;
		if (FEnvironmentVars && FEnvironmentVars->TryGetValue(APropName.UpperCase(), envValue))
		{
			AReplaceText = envValue;
			AHandled = true;
		}
		else
		{
			// Format message similar to Delphi version
			AReplaceText = System::Sysutils::Format("ENV_%s_NOT_FOUND", ARRAYOFCONST((APropName.UpperCase())));
            AHandled = true; // Important: Handle even if not found to prevent further processing
		}
	}
	// Handle dynamic system information
	else if (SameText(AObjectName, "system"))
	{
		if (SameText(APropName, "timestamp"))
		{
			AReplaceText = FormatDateTime("yyyy-mm-dd hh:nn:ss", Now());
			AHandled = true;
		}
		else if (SameText(APropName, "year"))
		{
			AReplaceText = FormatDateTime("yyyy", Now());
			AHandled = true;
		}
        else
		{
			AReplaceText = System::Sysutils::Format("SYSTEM_%s_NOT_FOUND", ARRAYOFCONST((APropName.UpperCase())));
            AHandled = true; // Handle even if not found
		}
	}
    // Note: The original simple 'year' handler is now covered by the 'system' block above.
}
//---------------------------------------------------------------------------

void TMainWebModule::InitControllers()
{
    FTasksController = std::make_unique<TTasksController>(WebStencilsEngine);
	FCustomersController = std::make_unique<TCustomersController>(WebStencilsEngine, Customers);
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

		// Normalize the path to handle potential variations
		FResourcesPath = System::Ioutils::TPath::GetFullPath(FResourcesPath);

        WebStencilsEngine->RootDirectory = System::Ioutils::TPath::Combine(FResourcesPath, "html");
        WebFileDispatcher->RootDirectory = WebStencilsEngine->RootDirectory;

		String dbPath = System::Ioutils::TPath::Combine(FResourcesPath, "data/database.sqlite3");

		// Ensure the Connection component is assigned and configured in the DFM
		if (Connection)
		{
			Connection->Params->Database = dbPath;
		}
		else
		{
			printf("Warning: TFDConnection component 'Connection' is not assigned.\n");
		}

		// Initialize environment variables dictionary
		FEnvironmentVars = std::make_unique<System::Generics::Collections::TDictionary__2<System::UnicodeString, System::UnicodeString>>();
		FEnvironmentVars->Add("APP_VERSION", "1.0.0");
		FEnvironmentVars->Add("APP_NAME", "WebStencils demo");
		FEnvironmentVars->Add("APP_EDITION", "WebBroker C++");
		FEnvironmentVars->Add("COMPANY_NAME", "Embarcadero Inc.");
#ifdef _DEBUG
		FEnvironmentVars->Add("DEBUG_MODE", "True");
#else
		FEnvironmentVars->Add("DEBUG_MODE", "False");
#endif

		// Controllers will handle providing data to templates.
        WebStencilsEngine->AddVar("customers", Customers, false);
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
        // Tasks Routes
        TRoute(mtDelete, "/tasks", FTasksController->DeleteTask),
        TRoute(mtPost, "/tasks/add", FTasksController->CreateTask),
        TRoute(mtGet, "/tasks/edit", FTasksController->GetEditTask),
        TRoute(mtPut, "/tasks/toggleCompleted", FTasksController->ToggleCompletedTask),
        TRoute(mtPut, "/tasks", FTasksController->EditTask),
		// Customers Routes
		TRoute(mtGet, "/bigtable", FCustomersController->GetAllCustomers),
		TRoute(mtGet, "/pagination", FCustomersController->GetCustomers)
    };

    // Ensure ClassHelpers.h defines TWebModuleHelper::AddRoutes correctly
    TWebModuleHelper::AddRoutes(this, routes);
}

//---------------------------------------------------------------------------



