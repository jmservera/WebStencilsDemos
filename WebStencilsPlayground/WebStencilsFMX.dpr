program WebStencilsFMX;

uses
  System.StartUpCopy,
  FMX.Forms,
  MainForm in 'MainForm.pas' {WebStencilsPlayground},
  PersonClass in 'PersonClass.pas',
  MyTemplates in 'MyTemplates.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TWebStencilsPlayground, WebStencilsPlayground);
  Application.Run;
end.
