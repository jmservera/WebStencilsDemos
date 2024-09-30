unit Unit310;

// EMS Resource Module

interface

uses
  System.SysUtils, System.Classes, System.JSON, System.Net.MIME,
  EMS.Services, EMS.ResourceAPI, EMS.ResourceTypes, EMS.FileResource,
  Data.DB, Datasnap.DBClient, Web.Stencils, Web.HTTPApp;

type
  [ResourceName('testfile')]
  TTestfileResource1 = class(TDataModule)
    [WebStencilsVar('dataset', False)]
    ClientDataSet1: TClientDataSet;

    [ResourceSuffix('./')]
    [ResourceSuffix('get', './')]
    [EndpointProduce('get', 'text/html')]
    html: TEMSFileResource;

    [ResourceSuffix('./')]
    [ResourceSuffix('get', './{filename}')]
    [EndpointProduce('get', 'text/html')]
    html2: TEMSFileResource;

    WebStencilsEngine1: TWebStencilsEngine;
    procedure DataModuleCreate(Sender: TObject);
    procedure WebStencilsEngine1Value(Sender: TObject; const AObjectName,
      APropName: string; var AReplaceText: string; var AHandled: Boolean);
    procedure WebStencilsEngine1Language(Sender: TObject;
      const APropName: string; var AReplaceText: string);
    procedure WebStencilsEngine1PathTemplates2PathInit(Sender: TObject;
      const ARequest: TWebPostProcessorRequest);
  end;

implementation

{%CLASSGROUP 'System.Classes.TPersistent'}

{$R *.dfm}

{ TSimpleObject }

type
  TSimpleObject = class
  private
    FName: string;
    FValue: Integer;
    function GetValueBelowTen: Boolean;
  public
    constructor Create(AName: string; AValue: Integer);
    property Name: string read FName write FName;
    property Value: Integer read FValue write FValue;
    property ValueBelowTen: Boolean read GetValueBelowTen;
  end;

constructor TSimpleObject.Create(AName: string; AValue: Integer);
begin
  inherited Create;
  FName := AName;
  FValue := AValue;
end;

function TSimpleObject.GetValueBelowTen: Boolean;
begin
  Result := Value < 10;
end;

{ TTestfileResource1 }

procedure TTestfileResource1.DataModuleCreate(Sender: TObject);
begin
  var AbsoluteProjectPath := 'C:\Users\azapater\Documents\GitHub\InternalWebStencilsDemos\WebStencilsRADServer';
  html.PathTemplate := AbsoluteProjectPath+'\html\{filename}';
  html2.PathTemplate := html.PathTemplate;
  AddProcessor(html, WebStencilsEngine1);
  WebStencilsEngine1.AddVar('dataO', TSimpleObject.Create('joe', 55));
  WebStencilsEngine1.AddVar('obj1', TSimpleObject.Create('joe', 55));
  WebStencilsEngine1.AddVar('obj2', TSimpleObject.Create('marc', 9));
end;

procedure Register;
begin
  RegisterResource(TypeInfo(TTestfileResource1));
end;

procedure TTestfileResource1.WebStencilsEngine1Value(Sender: TObject;
  const AObjectName, APropName: string; var AReplaceText: string;
  var AHandled: Boolean);
begin
  if SameText(AObjectName, 'dataE') and
     (SameText(APropName, 'name') or SameText(APropName, 'value')) then
  begin
    AHandled := True;
    AReplaceText := 'You requested ' + APropName;
  end;
end;

procedure TTestfileResource1.WebStencilsEngine1Language(Sender: TObject;
  const APropName: string; var AReplaceText: string);
begin
  if APropName = 'hellow' then
    AReplaceText := 'Hello World from Engine';
end;

procedure TTestfileResource1.WebStencilsEngine1PathTemplates2PathInit(
  Sender: TObject; const ARequest: TWebPostProcessorRequest);
begin
  var id := ARequest.Vars.Values['id'];
  if id <> '' then
    ClientDataSet1.Locate('CustNo', id, []);
end;

initialization
  Register;
end.


