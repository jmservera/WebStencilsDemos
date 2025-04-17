//---------------------------------------------------------------------------
#include "MainWebModule.h"
#include <IdHTTPWebBrokerBridge.hpp>
#include <IdContext.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TComponentClass WebModuleClass = __classid(TMainWebModule);
//---------------------------------------------------------------------------

__fastcall TMainWebModule::TMainWebModule(TComponent* Owner)
	: TWebModule(Owner)
{
	InitRequiredData();
    InitControllers();
	DefineRoutes();
}
//---------------------------------------------------------------------------

__fastcall TMainWebModule::~TMainWebModule()
{
    customers->Active = false;
    FCodeExamples.reset();
    FTasksController.reset();
}

void TMainWebModule::InitControllers()
{
    FTasksController = std::make_unique<TTasksController>(WebStencilsEngine);
}

void TMainWebModule::InitRequiredData()
{
    try {
		FResourcesPath = "../../../../resources";
        WebStencilsEngine->RootDirectory = FResourcesPath + "html/";
        WebFileDispatcher->RootDirectory = WebStencilsEngine->RootDirectory;
        
        // Initialize customers data
        try {
            customers->Active = false;
            String jsonPath = FResourcesPath + "data/customersFireDac.json";
            if (FileExists(jsonPath)) {
                customers->LoadFromFile(jsonPath, sfJSON);
            }
            customers->Active = true;
            WebStencilsEngine->AddVar("customers", customers, false);
        }
        catch (Exception& E) {
            printf("Warning: Could not load customers data: %s\n", AnsiString(E.Message).c_str());
            customers->Active = true; // Ensure table is active even if load fails
        }

        // Initialize code examples
        try {
            FCodeExamples = std::make_unique<TCodeExamples>(WebStencilsEngine);
        }
        catch (Exception& E) {
            printf("Warning: Could not initialize code examples: %s\n", AnsiString(E.Message).c_str());
            // Continue without code examples
        }
    }
    catch (Exception& E) {
        printf("Error in InitRequiredData: %s\n", AnsiString(E.Message).c_str());
        throw;
    }
}

void TMainWebModule::DefineRoutes()
{
    std::vector<TRoute> routes = {
        TRoute(mtDelete, "/tasks", FTasksController->DeleteTask),
        TRoute(mtPost, "/tasks/add", FTasksController->CreateTask),
        TRoute(mtGet, "/tasks/edit", FTasksController->GetEditTask),
        TRoute(mtPut, "/tasks/toggleCompleted", FTasksController->ToggleCompletedTask),
        TRoute(mtPut, "/tasks", FTasksController->EditTask)
    };
    TWebModuleHelper::AddRoutes(this, routes);
}

//---------------------------------------------------------------------------



