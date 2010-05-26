#ifndef __PeasDEMO_HELLO_WORLD_PLUGIN_H__
#define __PeasDEMO_HELLO_WORLD_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include <libpeas/peas-extension-base.h>
#include <libpeas/peas-plugin-info.h>
#include <libpeas/peas-object-module.h>

G_BEGIN_DECLS

#define PEASDEMO_TYPE_HELLO_WORLD_PLUGIN         (peasdemo_hello_world_plugin_get_type ())
#define PEASDEMO_HELLO_WORLD_PLUGIN(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), PeasDEMO_TYPE_HELLO_WORLD_PLUGIN, PeasDemoHelloWorldPlugin))
#define PEASDEMO_HELLO_WORLD_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), PeasDEMO_TYPE_HELLO_WORLD_PLUGIN, PeasDemoHelloWorldPlugin))
#define PEASDEMO_IS_HELLO_WORLD_PLUGIN(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), PeasDEMO_TYPE_HELLO_WORLD_PLUGIN))
#define PEASDEMO_IS_HELLO_WORLD_PLUGIN_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), PeasDEMO_TYPE_HELLO_WORLD_PLUGIN))
#define PEASDEMO_HELLO_WORLD_PLUGIN_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), PeasDEMO_TYPE_HELLO_WORLD_PLUGIN, PeasDemoHelloWorldPluginClass))

typedef struct _PeasDemoHelloWorldPlugin       PeasDemoHelloWorldPlugin;
typedef struct _PeasDemoHelloWorldPluginClass  PeasDemoHelloWorldPluginClass;

struct _PeasDemoHelloWorldPlugin {
  PeasExtensionBase parent_instance;
};

struct _PeasDemoHelloWorldPluginClass {
  PeasExtensionBaseClass parent_class;
};

GType                       peasdemo_hello_world_plugin_get_type  (void) G_GNUC_CONST;
G_MODULE_EXPORT void        peas_register_types                   (PeasObjectModule *module);

G_END_DECLS

#endif /* __PeasDEMO_HELLO_WORLD_PLUGIN_H__ */