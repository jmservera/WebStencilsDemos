//---------------------------------------------------------------------------
#pragma hdrstop

#include "ModelPaginationParams.h"
#include <System.SysUtils.hpp>
#include <System.JSON.hpp> // Include for TJSONObject if needed, or for TryGetValue

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TPaginationParams::TPaginationParams(TEndpointRequest* ARequest, System::UnicodeString AUri)
	: TObject()
{
	FUri = AUri; // Store URI if needed by the template
	ParsePaginationParams(ARequest);
}
//---------------------------------------------------------------------------

void TPaginationParams::ParsePaginationParams(TEndpointRequest* ARequest)
{
	System::UnicodeString LPageSizeStr, LPageNumberStr;

	// Initialize with default values
	FPageSize = DEFAULT_PAGE_SIZE;
	FPageNumber = DEFAULT_PAGE_NUMBER;

	// Try to get PageSize parameter from RAD Server request params
	// Note: TEndpointRequest::Params is often TStrings, check specific usage if different
	if (ARequest->Params->TryGetValue("pageSize", LPageSizeStr) && !LPageSizeStr.IsEmpty())
	{
		FPageSize = StrToIntDef(LPageSizeStr, DEFAULT_PAGE_SIZE);
		// Validate PageSize
		if (FPageSize <= 0)
			FPageSize = DEFAULT_PAGE_SIZE;
		else if (FPageSize > MAX_PAGE_SIZE)
			FPageSize = MAX_PAGE_SIZE;
	}

	// Try to get PageNumber parameter from RAD Server request params
	if (ARequest->Params->TryGetValue("page", LPageNumberStr) && !LPageNumberStr.IsEmpty())
	{
		FPageNumber = StrToIntDef(LPageNumberStr, DEFAULT_PAGE_NUMBER);
		// Validate PageNumber
		if (FPageNumber <= 0)
			FPageNumber = DEFAULT_PAGE_NUMBER;
	}
}
//--------------------------------------------------------------------------- 