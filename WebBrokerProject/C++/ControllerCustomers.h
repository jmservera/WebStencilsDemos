#ifndef ControllerCustomersH
#define ControllerCustomersH

#include <System.Classes.hpp>
#include <System.SysUtils.hpp>
#include <System.IOUtils.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <Web.HTTPApp.hpp>
#include <Web.Stencils.hpp> // Includes TWebStencilsEngine and TWebStencilsProcessor definitions
#include "ModelPaginationParams.h"
#include "FDQueryHelpers.h" // Include our helper functions

class TCustomersController : public TObject
{
private:
	TFDQuery* FCustomers; // Pointer to the FDQuery component (owned by DataModule/Form)
	Web::Stencils::TWebStencilsProcessor* FWebStencilsProcessor; // Owns this processor
	Web::Stencils::TWebStencilsEngine* FWebStencilsEngine; // Pointer to the engine (owned elsewhere)

	String RenderTemplate(String ATemplate, TPaginationParams* APaginationParams = nullptr);

public:
	void __fastcall GetCustomers(TObject* Sender, TWebRequest* Request, TWebResponse* Response, bool &Handled);
	void __fastcall GetAllCustomers(TObject* Sender, TWebRequest* Request, TWebResponse* Response, bool &Handled);

	__fastcall TCustomersController(Web::Stencils::TWebStencilsEngine* AWebStencilsEngine, TFDQuery* ACustomers);
	__fastcall ~TCustomersController();
};

#endif
