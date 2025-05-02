//---------------------------------------------------------------------------

#pragma hdrstop

#include "ControllerCustomers.h"
#include <Web.Stencils.hpp>
#include <System.IOUtils.hpp>
#include <System.SysUtils.hpp>
#include <memory>
#include <iostream>
#include <string>

//-----------------------------------------------------------------------------
#pragma package(smart_init)
//-----------------------------------------------------------------------------

__fastcall TCustomersController::TCustomersController(Web::Stencils::TWebStencilsEngine* AWebStencilsEngine, TFDQuery* ACustomers)
	: TObject()
{
	try
	{
		FWebStencilsEngine = AWebStencilsEngine;
		// Use fully qualified name for clarity and to avoid potential ambiguity
		FWebStencilsProcessor = new Web::Stencils::TWebStencilsProcessor(nullptr);
		FWebStencilsProcessor->Engine = FWebStencilsEngine;
		FCustomers = ACustomers; // Store pointer to existing query
	}
	catch (Exception &E)
	{
		std::cerr << "TCustomersController::Create: " << AnsiString(E.Message).c_str() << std::endl;
		throw;
	}
}
//-----------------------------------------------------------------------------

__fastcall TCustomersController::~TCustomersController()
{
	delete FWebStencilsProcessor;
}
//-----------------------------------------------------------------------------

String TCustomersController::RenderTemplate(String ATemplate, TPaginationParams* APaginationParams)
{
	String RootDir = FWebStencilsEngine->RootDirectory;
	if (!RootDir.IsEmpty() && *RootDir.LastChar() != System::Sysutils::PathDelim)
	{
		RootDir += System::Sysutils::PathDelim;
	}

	FWebStencilsProcessor->InputFileName = RootDir + "customers" + System::Sysutils::PathDelim + ATemplate + ".html";

	if (APaginationParams != nullptr)
	{
		FWebStencilsProcessor->AddVar("customersPagination", APaginationParams, false); // AddVar takes ownership if last param is true
	}

	String Result = FWebStencilsProcessor->Content();

	if (APaginationParams != nullptr)
	{
		FWebStencilsProcessor->DataVars->Remove("customersPagination");
	}
	return Result;
}
//-----------------------------------------------------------------------------

void __fastcall TCustomersController::GetCustomers(TObject* Sender, TWebRequest* Request, TWebResponse* Response, bool &Handled)
{
	std::unique_ptr<TPaginationParams> LPaginationParams(new TPaginationParams(Request, "pagination"));
	try
	{
		// Use the helper functions for pagination
		FDQueryHelpers::SetPageSize(FCustomers, LPaginationParams->PageSize);
		FDQueryHelpers::SetPageNumber(FCustomers, LPaginationParams->PageNumber);
		FDQueryHelpers::ApplyPagination(FCustomers); // Reopens the query with pagination settings

		// Get total pages *after* applying pagination and opening the query
		LPaginationParams->TotalPages = FDQueryHelpers::GetTotalPages(FCustomers);

		// Pass the raw pointer to RenderTemplate
		Response->Content = RenderTemplate("pagination", LPaginationParams.get());
		Handled = true;
	}
	catch (Exception &E)
	{
		// Handle potential exceptions during database access or template rendering
		Response->StatusCode = 500; // Internal Server Error
		Response->Content = "Error retrieving customers: " + E.Message;
		Handled = true;
		std::cerr << "TCustomersController::GetCustomers Error: " << AnsiString(E.Message).c_str() << std::endl;
	}
	// LPaginationParams is automatically deleted here by unique_ptr destructor
}
//-----------------------------------------------------------------------------

void __fastcall TCustomersController::GetAllCustomers(TObject* Sender, TWebRequest* Request, TWebResponse* Response, bool &Handled)
{
	try
	{
		FDQueryHelpers::CancelPagination(FCustomers); // Reopens the query without pagination limits
		Response->Content = RenderTemplate("bigtable", nullptr);
		Handled = true;
	}
	catch (Exception &E)
	{
		// Handle potential exceptions during database access or template rendering
		Response->StatusCode = 500; // Internal Server Error
		Response->Content = "Error retrieving all customers: " + E.Message;
		Handled = true;
		std::cerr << "TCustomersController::GetAllCustomers Error: " << AnsiString(E.Message).c_str() << std::endl;
	}
}
//-----------------------------------------------------------------------------
