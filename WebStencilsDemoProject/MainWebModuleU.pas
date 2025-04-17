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
	Controller.Customers;

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
	FResourcesPath := TPath.Combine(BinaryPath, '../../');
{$ELSE}
	FResourcesPath := BinaryPath;
{$ENDIF}
	WebStencilsEngine.RootDirectory := TPath.Combine(FResourcesPath, 'html');
	WebFileDispatcher.RootDirectory := WebStencilsEngine.RootDirectory;
	Connection.Params.Database := TPath.Combine(FResourcesPath, 'resources/data/database.sqlite3');
end;

procedure TMainWebModule.WebStencilsEngineValue(Sender: TObject;
	const AObjectName, APropName: string; var AReplaceText: string;
	var AHandled: Boolean);
begin
	if AObjectName = 'year' then
		AReplaceText := FormatDateTime('yyyy', Now);
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
