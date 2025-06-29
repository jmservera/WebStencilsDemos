﻿# WebStencils Demo Project

![WebStencils screenshot](../../.github/images/WebBroker.png)

## 🌟 Overview 
This project demonstrates the usage of WebStencils using WebBroker. It includes part of the documentation built into the generated website, as well as a couple of demos. 

## 🚦 Getting Started
The project can be run on Windows and Linux. 

On Windows, nothing needs to be done. Run and off you go. The default behavior is trying to get the HTML templates and database from the `resources` folder (preconfigured). On a production environment, the path should be changed.

To deploy on Linux change the `Build configuration` to `Release`. The `resources` folder needs to be copied to the same location where the binary is deployed. It needs PAServer running on a Linux machine (it also works with PAServer docker image).

## 📚 Examples
### Docs
Most of the menus explain the general use of WebStencils as well as some suggested ideas for templating patterns. 

### Big Table
This demo loads 1000 customers loaded in a FireDAC query. Customers data is stored in a `sqlite` database in `resources/data/database.sqlite3`.

### Pagination
Same `customers` table from the `sqlite` database, but this time using server-side pagination.

### To-Do app - HTMX Integration
This demo uses an in-memory `TList<TTaskItem>` managed by a singleton (`Model.Tasks.pas`) to store task data. It uses HTMX with WebStencils for dynamic content updates. See the `partials/tasks` templates and related Delphi code (`Controller.Tasks.pas`) for implementation details.

## 📁 Project Structure
The project consists of the following main components:
1. Delphi source files (`.pas` and `.dfm`)
2. **Shared** HTML templates (`.html`) located in `resources/html`
3. **Shared** Static assets (CSS, JavaScript, images) located in `resources/static`

### 🔑 Key Delphi Units
- `WebStencilsDemo.dpr`: The main project file that includes WebBroker
- `MainWebModuleU.pas`: Handles web requests and sets up the WebStencils engine
- `Model.Tasks.pas` and `Controller.Tasks.pas`: Implement the Tasks demo functionality (Tasks are stored in memory using a singleton)
- `Controller.Customers.pas`: contains the controller used for `Big Table` and `Pagination` demos. 
- `Model.PaginationParams.pas` defines a reusable pagination system for WebStencils. 
- `Helper` namespace: includes multiple class helpers to simplify some of the functionality like simplification for routing in a WebModule or pagination on a FDQuery. 
- `CodeExamplesU.pas`: Contains code examples used in the demo pages

### 📄 HTML Templates
Located in the **shared** `resources/html` directory. This demonstrates how the same templates can be used across different projects (Delphi/C++, WebBroker/RAD Server).
- `layouts/mainLayout.html`: The main layout template
- Various content pages (e.g., `home.html`, `basics.html`, `keywords.html`)
- Partial templates reusable across pages in the `partials/` directory
- `partials/customers`: Templates for the `Big Table` and `Pagination` demos.
- `partials/tasks`: Templates specific to the HTMX Tasks demo.

> **IMPORTANT**
> The `codeBlock` template has a `copy` button. Due to browser security limitations, this only works if the URL is `localhost` or if it's being run under https. If the demo is accessed through the network, the button is not functional.

## ✨ Environment Variables & Data Sources
- The `MainWebModuleU` demonstrates handling custom environment variables accessible in templates via `@env.VARIABLE_NAME` (e.g., `@env.APP_NAME`, `@env.APP_EDITION`).
- It also shows handling system variables like `@system.year` and `@system.timestamp` via the WebStencilsEngine `OnValue` event.
- The `DEBUG_MODE` variable is automatically set based on the build configuration.
- **Data Sources:** This project uses an in-memory list for Tasks and an SQLite database (`resources/data/database.sqlite3`) for Customer data, showcasing WebStencils' independence from specific data storage mechanisms.

## 💻 Web Tech Used
- Bootstrap 5.3
- Bootstrap icons
- HTMX 2.0.2
- Minor custom CSS and JS

All the external dependencies are loaded directly from CDNs. The custom CSS and JS can be found in the `static` folder.
