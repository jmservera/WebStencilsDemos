//---------------------------------------------------------------------------
#pragma hdrstop

#include "ControllerCustomers.h"
#include "ModelPaginationParams.h" // Include the C++ header
#include <Web.Stencils.hpp>
#include <System.IOUtils.hpp>
#include <System.SysUtils.hpp>
#include <System.JSON.hpp>     // For TryGetValue if used directly
#include <memory>             // For std::unique_ptr
#include <iostream>           // For basic error logging
#include <FireDAC.Comp.Client.hpp>
#include "FDQueryHelpers.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TCustomersController::TCustomersController(Web::Stencils::TWebStencilsEngine* AWebStencilsEngine, TFDQuery* ACustomers)
	: TObject()
{
	try
	{
		FWebStencilsEngine = AWebStencilsEngine;
		FWebStencilsProcessor = new Web::Stencils::TWebStencilsProcessor(nullptr);
		FWebStencilsProcessor->Engine = FWebStencilsEngine;
		FCustomers = ACustomers;
	}
	catch (Exception &E)
	{
		// Basic error logging - consider a more robust system
		std::cerr << "TCustomersController Constructor Error: " << AnsiString(E.Message).c_str() << std::endl;
		throw; // Re-throw
	}
}
//---------------------------------------------------------------------------

__fastcall TCustomersController::~TCustomersController()
{
	delete FWebStencilsProcessor; // Clean up owned processor
}
//---------------------------------------------------------------------------

// Overload for templates without pagination data
System::UnicodeString TCustomersController::RenderTemplate(System::UnicodeString ATemplate)
{
	// Use TPath::Combine for platform-independent path construction
	FWebStencilsProcessor->InputFileName = TPath::Combine(FWebStencilsEngine->RootDirectory,
		System::UnicodeString("customers") + System::Sysutils::PathDelim + ATemplate + ".html");
	return FWebStencilsProcessor->Content();
}
//---------------------------------------------------------------------------

// Overload for templates with pagination data
System::UnicodeString TCustomersController::RenderTemplate(System::UnicodeString ATemplate, TPaginationParams* APaginationParams)
{
	FWebStencilsProcessor->InputFileName = TPath::Combine(FWebStencilsEngine->RootDirectory,
		System::UnicodeString("customers") + System::Sysutils::PathDelim + ATemplate + ".html");

	if (APaginationParams != nullptr)
	{
		// AddVar takes ownership if the last param is true, here it's false.
		FWebStencilsProcessor->AddVar("customersPagination", APaginationParams, false);
	}

	System::UnicodeString Result = FWebStencilsProcessor->Content();

	if (APaginationParams != nullptr)
	{
		// RemoveVar just removes from the list, doesn't delete the object since OwnsObject was false.
		FWebStencilsProcessor->DataVars->Remove("customersPagination");
	}
	return Result;
}
//---------------------------------------------------------------------------

void __fastcall TCustomersController::GetCustomers(TEndpointRequest* ARequest, TEndpointResponse* AResponse)
{
	// Use unique_ptr for automatic cleanup of TPaginationParams
	std::unique_ptr<TPaginationParams> LPaginationParams;
	try
	{
		// Create pagination params object from request
		// Pass the URI string expected by the constructor (match Delphi)
		LPaginationParams = std::unique_ptr<TPaginationParams>(new TPaginationParams(ARequest, "pagination"));

		// Apply pagination using the Delphi helper via its generated HPP
		FDQueryHelpers::SetPageSize(FCustomers, LPaginationParams->PageSize);
		FDQueryHelpers::SetPageNumber(FCustomers, LPaginationParams->PageNumber);
		FDQueryHelpers::ApplyPagination(FCustomers); // Activates/refreshes query

		// Update the total pages in the params object *after* applying pagination
		LPaginationParams->TotalPages = FDQueryHelpers::GetTotalPages(FCustomers);

		// Render template and set RAD Server response
		// Pass the raw pointer using .get()
		AResponse->Body->SetString(RenderTemplate("pagination", LPaginationParams.get()));
	}
	catch (Exception &E)
	{
		std::cerr << "TCustomersController::GetCustomers Error: " << AnsiString(E.Message).c_str() << std::endl;
		AResponse->RaiseError(500, "Error retrieving customers", E.Message);
	}
	// unique_ptr automatically deletes LPaginationParams here, even if exceptions occurred
}
//---------------------------------------------------------------------------

void __fastcall TCustomersController::GetAllCustomers(TEndpointRequest* ARequest, TEndpointResponse* AResponse)
{
	try
	{
		// Use the Delphi helper via its generated HPP
		FDQueryHelpers::CancelPagination(FCustomers); // Deactivates/refreshes

		// Render template and set RAD Server response
		AResponse->Body->SetString(RenderTemplate("bigtable"));
	}
	catch (Exception &E)
	{
		std::cerr << "TCustomersController::GetAllCustomers Error: " << AnsiString(E.Message).c_str() << std::endl;
		AResponse->RaiseError(500, "Error retrieving all customers", E.Message);
	}
}
//--------------------------------------------------------------------------- 