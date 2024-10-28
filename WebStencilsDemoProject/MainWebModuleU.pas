unit MainWebModuleU;

interface

uses
  System.SysUtils, System.Classes, Web.HTTPApp, FireDAC.Stan.Intf, FireDAC.Stan.Option,
  FireDAC.Stan.Param, FireDAC.Stan.Error, FireDAC.DatS, FireDAC.Phys.Intf, FireDAC.DApt.Intf,
  Data.DB, FireDAC.Comp.DataSet, FireDAC.Comp.Client, Web.Stencils, FireDAC.Stan.StorageJSON,
  System.Generics.Collections,

  utils.ClassHelpers,
  Tasks.Controller,
  Tasks.Model,
  CodeExamplesU;

type
  TMainWebModule = class(TWebModule)
    WebStencilsEngine: TWebStencilsEngine;
    // Adding to WebStencils an object/component using attributes
    [WebStencilsVar('customers', false)]
    customers: TFDMemTable;
    FDStanStorageJSONLink1: TFDStanStorageJSONLink;
    WebFileDispatcher: TWebFileDispatcher;
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  private
    FTasksController: TTasksController;
    FCodeExamples: TCodeExamples;
    FResourcesPath: string;
    procedure DefineRoutes;
    procedure InitRequiredData;
    procedure InitControllers;
  public
    { Public declarations }
  end;

var
  WebModuleClass: TComponentClass = TMainWebModule;

implementation

{%CLASSGROUP 'System.Classes.TPersistent'}
{$R *.dfm}

constructor TMainWebModule.Create(AOwner: TComponent);
begin
  inherited;
  InitControllers;
	InitRequiredData;
  DefineRoutes;
end;

destructor TMainWebModule.Destroy;
begin
  customers.Active := False;
  FTasksController.Free;
  FCodeExamples.Free;
  inherited;
end;

procedure TMainWebModule.InitControllers;
begin
  FTasksController := TTasksController.Create(WebStencilsEngine);
  // More Controllers can be initialized here
end;

procedure TMainWebModule.InitRequiredData;
begin
  // Set the path for resources based on the platform
  // Check Project/Deployment. The required resources are added there for Linux Deployment using PAServer
  // For simplifying the demo, on Windows the path of the templates is on the same project folder. Release deployment would need a fix in the Path
  {$IFDEF MSWINDOWS}
  FResourcesPath := '../../';
  {$ELSE}
  FResourcesPath := './';
  {$ENDIF}
  WebStencilsEngine.RootDirectory := FResourcesPath + 'html/';
  WebfileDispatcher.RootDirectory := WebStencilsEngine.RootDirectory;
  customers.LoadFromFile(FResourcesPath + 'resources/data/customersFireDac.json', sfJSON);
  customers.Active := True;
  FCodeExamples := TCodeExamples.Create(WebStencilsEngine);
end;

procedure TMainWebModule.DefineRoutes;
begin
  // Define the application's routes using a declarative approach.
  //This class helper maps HTTP methods and paths to their respective handler methods.
  AddRoutes([
    TRoute.Create(mtDelete, '/tasks', FTasksController.DeleteTask),
    TRoute.Create(mtPost, '/tasks/add', FTasksController.CreateTask),
    TRoute.Create(mtGet, '/tasks/edit', FTasksController.GetEditTask),
    TRoute.Create(mtPut, '/tasks/toggleCompleted', FTasksController.TogglecompletedTask),
    TRoute.Create(mtPut, '/tasks', FTasksController.EditTask)
  ]);
end;

end.

