// TasksModel.h
#ifndef ModelTasksH
#define ModelTasksH

#include <System.hpp>
#include <System.Classes.hpp>
#include <System.SyncObjs.hpp>
#include <System.Generics.Collections.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <memory>

// Inherit from TPersistent, make properties published for RTTI/WebStencils
class TTaskItem : public TPersistent {
private:
    int FId;
    String FDescription;
    bool FCompleted;

public:
    TTaskItem(int AId, const String& ADescription, const bool ACompleted = false);

__published: // Properties for WebStencils RTTI access
	// Keep original properties if needed internally
	__property int Id = {read=FId};
	__property String Description = {read=FDescription, write=FDescription};
	__property bool Completed = {read=FCompleted, write=FCompleted};
};

class TTasks : public TObject {
private:
    TFDConnection* FDConnection;
	// TList__1<TObject*>* FItems; // Removed internal list
    // int FNextId; // Removed internal ID

    int __fastcall GetCount();
    int __fastcall GetCompletedCount();
	// Return type changed to match Delphi refactoring (list of specific type)
	TList__1<TObject*>* __fastcall GetAllTasks();
    // int __fastcall GetNextId(); // Removed

public:
    TTasks(TFDConnection* AFDConnection = nullptr);
    virtual ~TTasks();
    // Return value needs careful memory management by caller
    TTaskItem* FindTaskById(int AId);
    // Changed return type to void
    void AddTask(const String& ADescription);
    void EditTask(int AId, const String& ADescription);
    void DeleteTask(int AId);
    // Changed return type to void
    void ToggleCompletedTask(int AId);

__published: // Properties for WebStencils RTTI access
    __property int Count = {read=GetCount};
    __property int CompletedCount = {read=GetCompletedCount};
    // __property int NextId = {read=GetNextId}; // Removed
	// Return type changed
	__property TList__1<TObject*>* AllTasks = {read=GetAllTasks};
};

#endif 