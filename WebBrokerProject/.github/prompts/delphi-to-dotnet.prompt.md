---
mode: 'agent'
description: 'Complete Demo: Migrating Delphi projects to .NET with comprehensive guidance on language features, frameworks, and architectural patterns.'
tools: ['changes', 'codebase', 'editFiles', 'fetch', 'findTestFiles', 'githubRepo', 'openSimpleBrowser', 'problems', 'runCommands', 'runTasks', 'search', 'terminalLastCommand', 'testFailure', 'usages', 'github-global-docker', 'create_branch', 'create_issue', 'create_pull_request', 'get_commit', 'get_file_contents', 'get_issue', 'get_issue_comments', 'get_me', 'get_pull_request', 'get_pull_request_comments', 'get_pull_request_diff', 'get_pull_request_files', 'get_pull_request_reviews', 'get_pull_request_status', 'list_branches', 'list_issues', 'list_pull_requests', 'push_files', 'search_code', 'search_issues', 'search_repositories', 'update_issue', 'update_pull_request', 'update_pull_request_branch']
---

# Delphi to .NET Migration Assistant

This prompt file provides comprehensive guidance for migrating Delphi projects to .NET, covering all major language features, frameworks, and architectural patterns.

## 🚨 MANDATORY FIRST STEP: Create Migration Branch

**Before starting any migration work, you MUST:**

1. **Create a new Git branch** for the migration work
   - Use a descriptive name like `feature/delphi-to-dotnet-migration` or `migration/webbroker-to-aspnetcore`
   - This isolates migration work from the main codebase
   - Allows for safe experimentation and rollback if needed

2. **Execute these commands immediately:**
   ```bash
   git checkout -b [name of the branch]
   ```

3. **Verify you're on the correct branch** before proceeding:
   ```bash
   git branch --show-current
   ```

**⚠️ DO NOT PROCEED with any file changes until the branch is created and you've switched to it.**

---

## 🎯 Migration Scope

When helping with Delphi to .NET migration, consider these key areas:

### Language Features Migration

#### 1. **Object Pascal Syntax to C#**
- Convert `unit` declarations to `namespace` declarations
- Transform `interface`/`implementation` sections to class definitions
- Convert Pascal-style properties (`property Name: string read FName write FName`) to C# properties (`public string Name { get; set; }`)
- Migrate `class helper` extensions to C# extension methods
- Convert Pascal case conventions to C# conventions (PascalCase for public members, camelCase for private fields)

#### 2. **Type System Conversions**
- Map Delphi types to .NET types:
  - `Integer` → `int`
  - `string` → `string`
  - `Boolean` → `bool`
  - `TDateTime` → `DateTime`
  - `Currency` → `decimal`
  - `Variant` → `object` or `dynamic`
  - `TObjectList<T>` → `List<T>`
  - `TStringList` → `List<string>` or `Dictionary<string, string>`

#### 3. **Memory Management**
- Convert manual object destruction (`FreeAndNil`, `try/finally` blocks) to C# `using` statements or rely on garbage collection
- Replace `class constructor`/`class destructor` with static constructors and finalizers where appropriate
- Convert singleton patterns with manual locking to thread-safe implementations using `Lazy<T>` or concurrent collections

### Framework-Specific Migrations

#### 4. **WebBroker to ASP.NET Core**
- Convert `TWebModule` to `Controller` classes
- Transform `TWebRequest`/`TWebResponse` to `HttpRequest`/`HttpResponse` or action methods with parameters
- Migrate `WebDispatcher` routing to ASP.NET Core routing attributes (`[Route]`, `[HttpGet]`, `[HttpPost]`)
- Convert action handlers to controller actions:
  ```csharp
  // From: procedure HandleRequest(Sender: TObject; Request: TWebRequest; Response: TWebResponse; var Handled: Boolean);
  // To: [HttpPost] public IActionResult HandleRequest(RequestModel model)
  ```

#### 5. **FireDAC to Entity Framework Core**
- Replace `TFDConnection` with `DbContext`
- Convert `TFDQuery` to LINQ queries or Entity Framework methods
- Migrate connection strings to `appsettings.json`
- Transform FireDAC transactions to EF Core transactions
- Convert custom `TFDQueryHelper` pagination to built-in EF Core pagination (`Skip()`, `Take()`)

#### 6. **VCL/FMX to Blazor/ASP.NET Core MVC**
- Convert Delphi forms (`.dfm` files) to Razor views or Blazor components
- Transform component event handlers to Blazor event handling
- Migrate data binding from VCL to Blazor data binding syntax

### Web Development Patterns

#### 7. **Template Engine Migration**
- Convert WebStencils template processing to Razor syntax:
  - `{{variable}}` → `@Model.Variable`
  - Template loops → `@foreach` loops
  - Conditional rendering → `@if` statements
- Migrate template inheritance to Razor layouts (`_Layout.cshtml`)
- Convert partial templates to Razor partial views (`_PartialName.cshtml`)

#### 8. **HTMX Integration**
- Maintain HTMX functionality in ASP.NET Core by returning partial views
- Convert Delphi controller actions that return HTML fragments to return `PartialViewResult`
- Implement server-side pagination with HTMX using ASP.NET Core action filters

### Data Access Patterns

#### 9. **Repository Pattern Implementation**
- Convert Delphi model classes with database access to Repository pattern
- Implement dependency injection for data access
- Create interfaces for repositories to enable testing and loose coupling

#### 10. **Singleton Pattern Migration**
- Replace Delphi singleton implementations with .NET dependency injection
- Convert thread-safe singletons to scoped or transient services
- Use `IServiceCollection` for service registration

### Configuration and Deployment

#### 11. **Configuration Management**
- Move hardcoded paths and settings to `appsettings.json`
- Convert Delphi conditional compilation (`{$IFDEF}`) to .NET configuration profiles
- Implement options pattern for strongly-typed configuration

#### 12. **Cross-Platform Considerations**
- Convert Windows-specific code (`Winapi.Windows`) to cross-platform alternatives
- Replace Delphi RTL functions with .NET BCL equivalents
- Update file path handling to use `Path.Combine()` and proper separators

### Modern .NET Practices

#### 13. **Dependency Injection**
- Implement constructor injection for dependencies
- Register services in `Program.cs` or `Startup.cs`
- Use interfaces for abstraction and testability

#### 14. **Async/Await Pattern**
- Convert synchronous database operations to async equivalents
- Implement async controller actions for better scalability
- Use `Task<T>` and `async`/`await` for non-blocking operations

#### 15. **Error Handling and Logging**
- Replace Delphi exception handling with .NET structured logging
- Implement global exception handling middleware
- Use `ILogger<T>` for structured logging instead of `WriteLn`

### Testing Migration

#### 16. **Unit Testing**
- Convert DUnit tests to xUnit, NUnit, or MSTest
- Implement test doubles and mocking using frameworks like Moq
- Create integration tests for controllers and data access

### Security Considerations

#### 17. **Input Validation and Security**
- Implement ASP.NET Core model validation attributes
- Use CSRF protection middleware
- Apply proper HTML encoding (already present in original code with `TNetEncoding.HTML.Encode`)

## 📋 Migration Checklist

When migrating a Delphi project, follow this systematic approach:

### 🔄 STEP 0: Git Branch Setup (MANDATORY FIRST STEP)
**⚠️ CRITICAL: Always start by creating a new branch for the migration work**

- [ ] **Create a new branch** with a descriptive name (e.g., `feature/delphi-to-dotnet-migration`, `migration/webbroker-to-aspnetcore`)
- [ ] Switch to the new branch before making any changes
- [ ] Set up CI/CD pipeline for the migration branch if needed

**Example Git commands:**
```bash
git checkout -b feature/delphi-to-dotnet-migration
```

### 📊 STEP 1: Architecture Analysis
- [ ] Identify all Delphi units and their responsibilities
- [ ] Map data access patterns and dependencies
- [ ] Document web endpoints and routing

2. **Project Structure Setup**
   - [ ] Create new ASP.NET Core project
   - [ ] Set up folder structure (Controllers, Models, Views, Services)
   - [ ] Configure dependency injection container

3. **Data Layer Migration**
   - [ ] Convert FireDAC connections to Entity Framework Core
   - [ ] Create DbContext and entity models
   - [ ] Implement repository pattern if needed
   - [ ] Migrate database connection strings

4. **Business Logic Migration**
   - [ ] Convert Delphi model classes to C# classes
   - [ ] Implement business logic in service classes
   - [ ] Convert singleton patterns to dependency injection

5. **Web Layer Migration**
   - [ ] Convert WebModule actions to Controller actions
   - [ ] Migrate template processing to Razor views
   - [ ] Implement proper routing and parameter binding

6. **Configuration and Settings**
   - [ ] Move settings to appsettings.json
   - [ ] Implement options pattern for configuration
   - [ ] Set up environment-specific configurations

7. **Testing Implementation**
   - [ ] Create unit tests for business logic
   - [ ] Implement integration tests for controllers
   - [ ] Add end-to-end tests if applicable

## 🔧 Code Transformation Examples

### WebBroker Controller to ASP.NET Core Controller

**Delphi (Before):**
```pascal
procedure TTasksController.CreateTask(Sender: TObject; Request: TWebRequest; Response: TWebResponse; var Handled: Boolean);
begin
  var lTask := Request.ContentFields.Values['task'];
  lTask := TNetEncoding.HTML.Encode(lTask);
  var lNewTaskItem := FTasks.AddTask(lTask);
  Response.Content := RenderTemplate('card');
  Handled := True;
end;
```

**C# (After):**
```csharp
[HttpPost]
public async Task<IActionResult> CreateTask([FromForm] string task)
{
    var encodedTask = HttpUtility.HtmlEncode(task);
    var newTaskItem = await _tasksService.AddTaskAsync(encodedTask);
    return PartialView("_TaskCard");
}
```

### Singleton Pattern Migration

**Delphi (Before):**
```pascal
class function TTasks.GetInstance: TTasks;
begin
  if not Assigned(FInstance) then
  begin
    FLock.Enter;
    try
      if not Assigned(FInstance) then
        FInstance := TTasks.Create;
    finally
      FLock.Leave;
    end;
  end;
  Result := FInstance;
end;
```

**C# (After):**
```csharp
// In Program.cs
builder.Services.AddSingleton<ITasksService, TasksService>();

// Usage in controller
public class TasksController : Controller
{
    private readonly ITasksService _tasksService;
    
    public TasksController(ITasksService tasksService)
    {
        _tasksService = tasksService;
    }
}
```

## 🎯 Best Practices for Migration

**🚨 RULE #1: ALWAYS CREATE A NEW BRANCH FIRST**
Before any migration work begins, you MUST:
1. Create a new branch with a descriptive name (e.g., `feature/delphi-to-dotnet-migration`)
2. Switch to the new branch
3. Push the branch to the remote repository

**Migration Best Practices:**
1. **Incremental Migration**: Migrate one module at a time to reduce risk
2. **Maintain Functionality**: Ensure feature parity during migration
3. **Modern Patterns**: Adopt .NET best practices and design patterns
4. **Testing**: Implement comprehensive tests during migration
5. **Performance**: Leverage async/await for better scalability
6. **Security**: Apply modern security practices from the start


## 📚 Additional Resources

- ASP.NET Core documentation for web development patterns
- Entity Framework Core for data access migration
- Blazor for rich client-side functionality
- xUnit for testing framework migration
- AutoMapper for object-to-object mapping

When providing migration assistance, always consider the specific context of the Delphi project and suggest the most appropriate .NET equivalent while following modern development practices.