#ifndef ClassHelpersH
#define ClassHelpersH

#include <System.SysUtils.hpp>
#include <Web.HTTPApp.hpp>
#include <vector>

// Route structure definition
struct TRoute {
    TMethodType MethodType;
    String PathInfo;
    THTTPMethodEvent OnAction;
    bool Default;

    TRoute(TMethodType AMethodType, const String& APathInfo,
           THTTPMethodEvent AOnAction, bool ADefault = false)
        : MethodType(AMethodType), PathInfo(APathInfo),
          OnAction(AOnAction), Default(ADefault) {}
};

// Approach 1: Using inheritance
class TWebModuleExtended : public TWebModule {
public:
    TWebModuleExtended(TComponent* Owner) : TWebModule(Owner) {}

    TWebModuleExtended* AddAction(TMethodType AMethodType,
                                 const String& APathInfo,
                                 THTTPMethodEvent AOnAction,
                                 bool ADefault = false);
    void AddRoutes(const std::vector<TRoute>& ARoutes);
};

// Approach 2: Using a utility class
class TWebModuleHelper {
public:
    static void AddRoutes(TWebModule* WebModule, const std::vector<TRoute>& Routes);
    static TWebModule* AddAction(TWebModule* WebModule, TMethodType AMethodType, const String& APathInfo,
        THTTPMethodEvent AOnAction, bool ADefault = false);
};

#endif
