unit Model.PaginationParams;

interface

uses
	System.SysUtils,
	Web.HTTPApp;

type

	TPaginationParams = class
		FPageSize: Integer;
		FPageNumber: Integer;
		FTotalPages: Integer;
		FUri: string;
	private
		procedure ParsePaginationParams(Request: TWebRequest);
		const
			DEFAULT_PAGE_SIZE = 10;
			DEFAULT_PAGE_NUMBER = 1;
			MAX_PAGE_SIZE = 100; // Prevent excessive page sizes
	public
		constructor Create(ARequest: TWebRequest; AUri: string);
		property PageSize: integer read FPageSize;
		property PageNumber: integer read FPageNumber;
		property TotalPages: integer read FTotalPages write FTotalPages;
    property Uri: string read FUri;
	end;

implementation

{ TPaginationParams }

constructor TPaginationParams.Create(ARequest: TWebRequest; AUri: string);
begin
	FUri := AUri;
	ParsePaginationParams(ARequest);
end;

procedure TPaginationParams.ParsePaginationParams(Request: TWebRequest);
var
  PageSizeStr, PageNumberStr: string;
begin
	// Initialize with default values
	FPageSize := DEFAULT_PAGE_SIZE;
	FPageNumber := DEFAULT_PAGE_NUMBER;

	// Try to get PageSize parameter
	PageSizeStr := Request.QueryFields.Values['size'];
  if PageSizeStr <> '' then
  begin
    try
			FPageSize := StrToInt(PageSizeStr);

      // Validate PageSize
			if FPageSize <= 0 then
				FPageSize := DEFAULT_PAGE_SIZE
			else if FPageSize > MAX_PAGE_SIZE then
				FPageSize := MAX_PAGE_SIZE;
    except
      on E: EConvertError do
				FPageSize := DEFAULT_PAGE_SIZE;
    end;
  end;

  // Try to get PageNumber parameter
	PageNumberStr := Request.QueryFields.Values['page'];
  if PageNumberStr <> '' then
  begin
    try
			FPageNumber := StrToInt(PageNumberStr);

      // Validate PageNumber
			if FPageNumber <= 0 then
				FPageNumber := DEFAULT_PAGE_NUMBER;
    except
      on E: EConvertError do
				FPageNumber := DEFAULT_PAGE_NUMBER;
    end;
  end;
end;

end.
