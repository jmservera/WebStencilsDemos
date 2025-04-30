//---------------------------------------------------------------------------
#ifndef MainWebModuleH
#define MainWebModuleH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Web.HTTPApp.hpp>
#include <Web.Stencils.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.StorageJSON.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.SQLite.hpp>
#include <FireDAC.Phys.SQLiteDef.hpp>
#include <FireDAC.Phys.SQLiteWrapper.Stat.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.ExprFuncs.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <memory> // Required for std::unique_ptr
#include <System.Generics.Collections.hpp> // Required for TDictionary

// Include own units/controllers
#include "CodeExamplesU.h"
#include "ClassHelpers.h"
#include "ControllerTasks.h"
#include "ControllerCustomers.h"

// Other FireDAC includes (cleaned up duplicates)
#include <FireDAC.DApt.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.SQLite.hpp>
#include <FireDAC.Phys.SQLiteDef.hpp>
#include <FireDAC.Phys.SQLiteWrapper.Stat.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.ExprFuncs.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>

//---------------------------------------------------------------------------
class TMainWebModule : public TWebModule
{
__published:	// IDE-managed Components
	TWebStencilsEngine *WebStencilsEngine;
	TWebFileDispatcher *WebFileDispatcher;
	TFDQuery *Customers;
	TFDConnection *Connection;

private:	// User declarations
	std::unique_ptr<TTasksController> FTasksController;
	std::unique_ptr<TCustomersController> FCustomersController;
	std::unique_ptr<TCodeExamples> FCodeExamples;
	std::unique_ptr<System::Generics::Collections::TDictionary__2<System::UnicodeString, System::UnicodeString>> FEnvironmentVars; // Added environment variables dictionary
	String FResourcesPath;

	void DefineRoutes();
	void InitRequiredData();
	void InitControllers();
	void __fastcall WebStencilsEngineValue(TObject* Sender, const String AObjectName,
			const String APropName, String &AReplaceText, bool &AHandled);

public:		// User declarations
	__fastcall TMainWebModule(TComponent* Owner);
	__fastcall virtual ~TMainWebModule();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWebModule *MainWebModule;
//---------------------------------------------------------------------------
#endif


