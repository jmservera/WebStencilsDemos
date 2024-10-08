unit WebRresource;

// EMS Resource Module

interface

uses
	System.SysUtils, System.Classes, System.JSON,
	EMS.Services, EMS.ResourceAPI, EMS.ResourceTypes,
	Web.Stencils, Web.HTTPApp, EMS.FileResource, FireDAC.Stan.Intf,
	FireDAC.Stan.Option, FireDAC.Stan.Error, FireDAC.UI.Intf,
	FireDAC.Phys.Intf, FireDAC.Stan.Def, FireDAC.Stan.Pool,
	FireDAC.Stan.Async, FireDAC.Phys, FireDAC.ConsoleUI.Wait, Data.DB,
	FireDAC.Comp.Client, FireDAC.Phys.IB, FireDAC.Phys.IBDef,
	FireDAC.Stan.Param, FireDAC.DatS, FireDAC.DApt.Intf, FireDAC.Comp.DataSet,

	CodeExamplesU,
	Tasks.Controller, FireDAC.DApt;

type
	[ResourceName('web')]
	TTasksResource1 = class(TDataModule)
//		[ResourceSuffix('./')]
		[ResourceSuffix('get', '/{filename}')]
		html: TEMSFileResource;
		[ResourceSuffix('get', '/css/{filename}')]
		css: TEMSFileResource;
		[ResourceSuffix('get', '/js/{filename}')]
		js: TEMSFileResource;
		[ResourceSuffix('get', '/img/{filename}')]
		img: TEMSFileResource;
		WebStencilsEngine1: TWebStencilsEngine;
		WebStencilsProcessor: TWebStencilsProcessor;
		FDConnection: TFDConnection;
		[WebStencilsVar('customers', false)]
		customers: TFDQuery;
		procedure DataModuleCreate(Sender: TObject);
	private
		FCodeExamples: TCodeExamples;
		FTasksController: TTasksController;
	published
		[ResourceSuffix('/')]
		procedure Get(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
		[ResourceSuffix('/tasks')]
		procedure DeleteTask(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
		[ResourceSuffix('/tasks/add')]
		procedure PostTask(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
		[ResourceSuffix('/tasks/edit')]
		procedure GetTasksEdit(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
		[ResourceSuffix('/tasks/toggleCompleted')]
		procedure PutTaskToggleCompleted(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
		[ResourceSuffix('/tasks')]
		procedure PutTask(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
	end;

implementation

{%CLASSGROUP 'System.Classes.TPersistent'}

{$R *.dfm}

procedure TTasksResource1.DataModuleCreate(Sender: TObject);
const
	// Replace this path with wherever you copy the project in your computer
	LProjectPath: string = 'C:\path\to\the\project\';
begin
	FDConnection.Params.Database := LProjectPath + 'resources\data\tasks.ib';
	html.PathTemplate := LProjectPath + 'html\{filename}';
	css.PathTemplate := LProjectPath + 'html\static\css\{filename}';
	js.PathTemplate := LProjectPath + 'html\static\js\{filename}';
	img.PathTemplate := LProjectPath + 'html\static\img\{filename}';
	WebStencilsProcessor.PathTemplate := LProjectPath + 'html\';
	AddProcessor(html, WebStencilsEngine1);
	FCodeExamples := TCodeExamples.Create(WebStencilsEngine1);
	FTasksController := TTasksController.Create(WebStencilsEngine1, FDConnection);
end;

procedure TTasksResource1.Get(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
var
	LHTMLContent: string;
begin
	WebStencilsProcessor.InputFileName := WebStencilsProcessor.PathTemplate + 'home.html';
	LHTMLContent := WebStencilsProcessor.Content;
	AResponse.Body.SetString(LHTMLContent);
end;

procedure TTasksResource1.DeleteTask(const AContext: TEndpointContext;
	const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
begin
	FtasksController.DeleteTask(ARequest, AResponse);
end;

procedure TTasksResource1.PostTask(const AContext: TEndpointContext;
	const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
begin
	FtasksController.CreateTask(ARequest, AResponse);
end;

procedure TTasksResource1.GetTasksEdit(const AContext: TEndpointContext; const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
begin
	FTasksController.GetEditTask(ARequest, Aresponse);
end;

procedure TTasksResource1.PutTaskToggleCompleted(
	const AContext: TEndpointContext; const ARequest: TEndpointRequest;
	const AResponse: TEndpointResponse);
begin
	FtasksController.TogglecompletedTask(ARequest, AResponse);
end;

procedure TTasksResource1.PutTask(const AContext: TEndpointContext;
	const ARequest: TEndpointRequest; const AResponse: TEndpointResponse);
begin
	FtasksController.EditTask(ARequest, AResponse);
end;

procedure Register;
begin
	RegisterResource(TypeInfo(TTasksResource1));
end;

initialization
	Register;
end.


