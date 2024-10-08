{
  This unit defines the model/repository for the Tasks.
  It implements a singleton pattern to manage a list of tasks.
}

unit Tasks.Model;

interface

uses
	System.Generics.Collections,
	System.SysUtils,
	System.SyncObjs,
	FireDAC.Comp.Client;

type
	TTaskItem = class
  private
    FId: Integer;
    FDescription: string;
    FCompleted: Boolean;
	public
		constructor Create(AId: Integer; const ADescription: string; const Acompleted: boolean = false);
    property Id: Integer read FId;
    property Description: string read FDescription write FDescription;
    property Completed: Boolean read FCompleted write FCompleted;
  end;

  TTasks = class
  private
		FDConnection: TFDConnection;
		FItems: TObjectList<TTaskItem>;
    FNextId: Integer;

		function GetCount: Integer;
		function GetCompletedCount: Integer;
		function GetAllTasks: TObjectList<TTaskItem>;
		function GetNextId: Integer;
	public
		constructor Create(AFDConnection: TFDConnection);
		destructor Destroy; override;
    function FindTaskById(AId: Integer): TTaskItem;
    function AddTask(const ADescription: string): TTaskItem;
    procedure EditTask(AId: Integer; ADescription: string);
    procedure DeleteTask(AId: Integer);
    function ToggleCompletedTask(AId: Integer): TTaskItem;
    property Count: Integer read GetCount;
    property CompletedCount: Integer read GetCompletedCount;
    property NextId: Integer read GetNextId;
    property AllTasks: TObjectList<TTaskItem> read GetAllTasks;
  end;

implementation

{ TTaskItem }

constructor TTaskItem.Create(AId: Integer; const ADescription: string; const Acompleted: boolean = false);
begin
	inherited Create;
	FId := AId;
	FDescription := ADescription;
	FCompleted := Acompleted;
end;

{ TTasks }

constructor TTasks.Create(AFDConnection: TFDConnection);
begin
  inherited Create;
	FItems := TObjectList<TTaskItem>.Create(True);
  FDConnection := AFDConnection;
  FNextId := 1;
end;

destructor TTasks.Destroy;
begin
	FItems.Free;
  FDConnection.Free;
  inherited;
end;

function TTasks.GetNextId: Integer;
begin
  Result := FNextId;
end;

procedure TTasks.EditTask(AId: Integer; ADescription: string);
var
  Item: TTaskItem;
begin
	Item := FindTaskById(AId);
	if Assigned(Item) then
		begin
			FDConnection.ExecSQL('UPDATE tasks SET description = :description WHERE id = :id', [ADescription, Item.Id]);
			Item.Description := ADescription;
		end;
end;

function TTasks.AddTask(const ADescription: string): TTaskItem;
var
	NewItem: TTaskItem;
begin
	FDConnection.ExecSQL('INSERT INTO tasks(id, description) VALUES (:id, :description)', [FNextId, ADescription]);
	NewItem := TTaskItem.Create(FNextId, ADescription);
	FItems.Add(NewItem);
	Result := NewItem;
	Inc(FNextId);
end;

function TTasks.ToggleCompletedTask(AId: Integer): TTaskItem;
var
	Item: TTaskItem;
begin
	Item := FindTaskById(AId);
	if Assigned(Item) then
	begin
		FDConnection.ExecSQL('EXECUTE PROCEDURE toggle_completed_task :id', [Item.Id]);
		Item.Completed := not(Item.Completed);
		Exit(Item);
	end;
	Result := nil;
end;

procedure TTasks.DeleteTask(AId: Integer);
var
	Item: TTaskItem;
begin
	Item := FindTaskById(AId);
	if Assigned(Item) then
		begin
			FDConnection.ExecSQL('DELETE FROM tasks WHERE id = :1', [Item.Id]);
			FItems.Remove(Item);
		end;
end;

function TTasks.GetCompletedCount: Integer;
var
  Item: TTaskItem;
begin
  Result := 0;
  for Item in FItems do
    if Item.Completed then
      Inc(Result);
end;

function TTasks.GetAllTasks: TObjectList<TTaskItem>;
var
	NewItem: TTaskItem;
	LQuery: TFDQuery;
begin
	LQuery := TFDQuery.Create(nil);
	try
		LQuery.Connection := FDConnection;
		LQuery.SQL.Text := 'SELECT * FROM tasks ORDER BY id';
		LQuery.Open;
		FNextId := 0;
		while not(LQuery.eof) do
			begin
				var id := LQuery.FieldByName('id').AsInteger;
				var description := LQuery.FieldByName('description').AsString;
				var completed := LQuery.FieldByName('completed').AsBoolean;
				NewItem := TTaskItem.Create(id, description, completed);
				FItems.Add(NewItem);
				FNextId := LQuery.fieldByName('id').AsInteger;
				LQuery.Next;
			end;
    inc(FNextId);
		Result := FItems;
		LQuery.Close;
	finally
		LQuery.Free;
	end;
end;

function TTasks.GetCount: Integer;
begin
  Result := FItems.Count;
end;

function TTasks.FindTaskById(AId: Integer): TTaskItem;
var
  Item: TTaskItem;
begin
  for Item in FItems do
    if Item.Id = AId then
      Exit(Item);
  Result := nil;
end;

end.
