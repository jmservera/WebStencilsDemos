//---------------------------------------------------------------------------
#include "MainWebModule.h"
#include <IdHTTPWebBrokerBridge.hpp>
#include <IdContext.hpp>
#include <System.IOUtils.hpp>
#include <System.DateUtils.hpp>
#include <System.SysUtils.hpp>
#include <vector>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"

TComponentClass WebModuleClass = __classid(TMainWebModule);
//---------------------------------------------------------------------------

// Implementation for TEnvironmentSettings constructor
__fastcall TEnvironmentSettings::TEnvironmentSettings()
    : System::Classes::TPersistent()
{
    FAppVersion = "1.0.0";
    FAppName = "WebStencils demo";
    FAppEdition = "WebBroker C++";
    FCompanyName = "Embarcadero Inc.";
    FResource = ""; // Set empty for WebBroker - This is needed to share the same templates between WebBroker and RadServer
    FIsRadServer = false;
#ifdef _DEBUG
    FDebugMode = true;
#else
    FDebugMode = false;
#endif
}

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
    // Controllers and FEnvironmentSettings are destroyed automatically by unique_ptr
}
//----------------------------------------------------------------------------
void TMainWebModule::WebStencilsEngineValue(TObject* Sender, const String AObjectName,
    const String APropName, String &AReplaceText, bool &AHandled)
{
    // env object is now handled by RTTI via AddVar

    // Handle dynamic system information
    if (SameText(AObjectName, "system"))
    {
        AHandled = true; // Handle all system properties here
        if (SameText(APropName, "timestamp"))
        {
            AReplaceText = FormatDateTime("yyyy-mm-dd hh:nn:ss", Now());
        }
        else if (SameText(APropName, "year"))
        {
            AReplaceText = FormatDateTime("yyyy", Now());
        }
        else
        {
            AReplaceText = System::Sysutils::Format("SYSTEM_%s_NOT_FOUND", ARRAYOFCONST((APropName.UpperCase())));
        }
    }
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
        // extract the binary location and point to the resources folder where the database and WebStencils templates are
        FResourcesPath = System::Ioutils::TPath::Combine(System::Ioutils::TPath::GetDirectoryName(GetModuleName(0)), "../../../../resources");

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

        // Initialize environment settings object
        FEnvironmentSettings = std::make_unique<TEnvironmentSettings>();
        // Add the settings object instance to the engine
        // Note: .get() passes the raw pointer, ownership remains with unique_ptr
        WebStencilsEngine->AddVar("env", FEnvironmentSettings.get());

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
