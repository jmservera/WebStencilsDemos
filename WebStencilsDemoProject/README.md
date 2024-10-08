# WebStencils Demo Project

![WebStencils screenshot](../.github/images/WebBroker.png)

## 🌟 Overview 
This project demonstrates the usage of WebStencils using WebBroker. It includes part of the documentation built into the generated website, as well as a couple of demos. 

## 🚦 Getting Started
The project can be run on Windows and Linux. 

On Windows, nothing needs to be done. Run and off you go. The default behavior is trying to get the HTML templates in the same path as the dpr. On a Release environment, the path should be changed. 

To deploy on Linux change the `Build configuration` to `Release`. All the required files are preconfigured in the `Deployment` window. It only needs PAServer running on a Linux machine (it also works with PAServer docker image). Connected to PAServer, run the application or deploy it and execute it directly from the Linux Machine.  

**IMPORTANT**: The codeBlock snippet has a `copy` button. Due to browser security limitations, this only works if the URL is "localhost" or if it's being run under https. If the demo is accessed through the network, the button is not functional. 

## 📚 Examples 
### Docs 
Most of the menus explain the general use of WebStencils as well as some suggested ideas for templating patterns. 

### Customers
This demo loads 1000 customers pre-loaded in a FDMemTable. The customers are stored in a JSON file in `resources/data`.

### To-Do app - HTMX Integration
The project includes an example of using HTMX with WebStencils for dynamic content updates. See the `tasks.html` template and related Delphi code for implementation details.

## 📁 Project Structure 
The project consists of the following main components:
1. Delphi source files (`.pas` and `.dfm`)
2. HTML templates (`.html`)
3. Static assets (CSS, JavaScript, images)

### 🔑 Key Delphi Units
- `WebStencilsDemo.dpr`: The main project file that includes WebBroker
- `MainWebModuleU.pas`: Handles web requests and sets up the WebStencils engine
- `Tasks.Model.pas` and `Tasks.Controller.pas`: Implement the Tasks demo functionality (Tasks are stored in memory using a singleton)
- `CodeExamplesU.pas`: Contains code examples used in the demo pages

### 📄 HTML Templates
- `layouts/mainLayout.html`: The main layout template
- Various content pages (e.g., `home.html`, `basics.html`, `keywords.html`)
- Partial templates in the `partials/` directory

## 💻 Web Tech Used
- Bootstrap 5.3
- Bootstrap icons
- HTMX 2.0.2
- Minor custom CSS and JS 

All the external dependencies are loaded directly from CDNs. The custom CSS and JS can be found in the `static` folder. 
