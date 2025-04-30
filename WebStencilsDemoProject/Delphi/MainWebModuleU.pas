unit MainWebModuleU;

interface

uses
	 // System units
	System.Classes,
	System.Generics.Collections,
	System.IOUtils,
	System.SysUtils,

	// Data units
	Data.DB,

	// Web units
	Web.HTTPApp,
	Web.Stencils,

	// FireDAC
	FireDAC.Stan.Async,
	FireDAC.Stan.Def,
	FireDAC.Stan.Error,
	FireDAC.Stan.ExprFuncs,
	FireDAC.Stan.Intf,
	FireDAC.Stan.Option,
	FireDAC.Stan.Param,
	FireDAC.Stan.Pool,
	FireDAC.Stan.StorageJSON,
	FireDAC.DApt,
	FireDAC.DApt.Intf,
	FireDAC.DatS,
	FireDAC.Phys,
	FireDAC.Phys.Intf,
	FireDAC.Phys.SQLite,
	FireDAC.Phys.SQLiteDef,
	FireDAC.Phys.SQLiteWrapper.Stat,
	FireDAC.ConsoleUI.Wait,
	FireDAC.Comp.Client,
	FireDAC.Comp.DataSet,
	FireDAC.UI.Intf,

	// Own units
	Helpers.WebModule,
	Helpers.FDQuery,
	Controller.Tasks,
	Model.Tasks,
	Controller.Customers,
  CodeExamplesU;

type
	TMainWebModule = class(TWebModule)
		WebStencilsEngine: TWebStencilsEngine;
		// Adding to WebStencils an object/component using attributes
		WebFileDispatcher: TWebFileDispatcher;
		[WebStencilsVar('customers', false)]
		Customers: TFDQuery;
		Connection: TFDConnection;
		constructor Create(AOwner: TComponent); override;
		destructor Destroy; override;
		procedure WebStencilsEngineValue(Sender: TObject;
			const AObjectName, APropName: string; var AReplaceText: string;
			var AHandled: Boolean);
	private
		FTasksController: TTasksController;
		FCustomersController: TCustomersController;
    FCodeExamples: TCodeExamples;
    FEnvironmentVars: TDictionary<string, string>;
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
	Customers.Active := false;
	FTasksController.Free;
	FCustomersController.Free;
  FCodeExamples.Free;
  FEnvironmentVars.Free;
	inherited;
end;

procedure TMainWebModule.InitControllers;
begin
	FTasksController := TTasksController.Create(WebStencilsEngine);
	FCustomersController := TCustomersController.Create(WebStencilsEngine, Customers);
	// More Controllers can be initialized here
end;

procedure TMainWebModule.InitRequiredData;
begin
	// Set the path for resources based on the platform
	// Check Project/Deployment. The required resources are added there for Linux Deployment using PAServer
	// For simplifying the demo, on Windows the path of the templates is on the same project folder. Release deployment would need a fix in the Path
	var BinaryPath := TPath.GetDirectoryName(ParamStr(0));
{$IFDEF MSWINDOWS}
	FResourcesPath := TPath.Combine(BinaryPath, '../../../../resources');
{$ELSE}
	FResourcesPath := BinaryPath;
{$ENDIF}
	WebStencilsEngine.RootDirectory := TPath.Combine(FResourcesPath, 'html');
	WebFileDispatcher.RootDirectory := WebStencilsEngine.RootDirectory;
	Connection.Params.Database := TPath.Combine(FResourcesPath, 'data/database.sqlite3');
  FCodeExamples := TCodeExamples.Create(WebStencilsEngine);

  FEnvironmentVars := TDictionary<string, string>.Create;
  // Initialize some environment variables
  FEnvironmentVars.Add('APP_VERSION', '1.0.0');
  FEnvironmentVars.Add('APP_NAME', 'WebStencils demo');
  FEnvironmentVars.Add('APP_EDITION', 'WebBroker Delphi');
  FEnvironmentVars.Add('COMPANY_NAME', 'Embarcadero Inc.');
{$IFDEF DEBUG}
  FEnvironmentVars.Add('DEBUG_MODE', 'True');
{$ELSE}
  FEnvironmentVars.Add('DEBUG_MODE', 'False');
{$ENDIF}
end;

procedure TMainWebModule.WebStencilsEngineValue(Sender: TObject;
  const AObjectName, APropName: string; var AReplaceText: string;
  var AHandled: Boolean);
begin
  // Check if we're accessing environment variables
  if SameText(AObjectName, 'env') then
  begin
    if FEnvironmentVars.TryGetValue(APropName.ToUpper, AReplaceText) then
      AHandled := True
    else
      AReplaceText := Format('ENV_%s_NOT_FOUND', [APropName.ToUpper]);
  end
  // Handle dynamic system information
  else if SameText(AObjectName, 'system') then
  begin
    if SameText(APropName, 'timestamp') then
    begin
      AReplaceText := FormatDateTime('yyyy-mm-dd hh:nn:ss', Now);
      AHandled := True;
    end
    else if SameText(APropName, 'year') then
    begin
      AReplaceText := FormatDateTime('yyyy', Now);
      AHandled := True;
    end
    else
      AReplaceText := Format('SYSTEM_%s_NOT_FOUND', [APropName.ToUpper]);
  end;
end;

procedure TMainWebModule.DefineRoutes;
begin
	// Define the application's routes using a declarative approach.
	// This class helper maps HTTP methods and paths to their respective handler methods.
	AddRoutes([TRoute.Create(mtDelete, '/tasks', FTasksController.DeleteTask),
		TRoute.Create(mtPost, '/tasks/add', FTasksController.CreateTask),
		TRoute.Create(mtGet, '/tasks/edit', FTasksController.GetEditTask),
		TRoute.Create(mtPut, '/tasks/toggleCompleted', FTasksController.TogglecompletedTask),
    TRoute.Create(mtPut, '/tasks', FTasksController.EditTask),
		// Customers routes
		TRoute.Create(mtGet, '/bigtable', FCustomersController.GetAllCustomers),
		TRoute.Create(mtGet, '/pagination', FCustomersController.GetCustomers)
		]);
end;

end.
