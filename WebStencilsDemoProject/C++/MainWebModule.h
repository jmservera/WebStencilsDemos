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
#include "CodeExamplesU.h"
#include "ClassHelpers.h"
#include "TasksController.h"
//---------------------------------------------------------------------------
class TMainWebModule : public TWebModule
{
__published:	// IDE-managed Components
	TWebStencilsEngine *WebStencilsEngine;
	TWebFileDispatcher *WebFileDispatcher;
	TFDMemTable *customers;
	TFDStanStorageJSONLink *FDStanStorageJSONLink1;

private:	// User declarations
	std::unique_ptr<TTasksController> FTasksController;
	std::unique_ptr<TCodeExamples> FCodeExamples;
	void DefineRoutes();
	void InitRequiredData();
	void InitControllers();
	String FResourcesPath;

public:		// User declarations
	__fastcall TMainWebModule(TComponent* Owner);
	__fastcall virtual ~TMainWebModule();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWebModule *MainWebModule;
//---------------------------------------------------------------------------
#endif


