//
// Created by rick on 2022/2/11.
//
#include <glib-object.h>
#define T_TYPE_DOUBLE (t_double_get_type())

typedef struct _TDouble TDouble;
struct _TDouble {
    GObject parent;
    double value;
};

typedef struct _TDoubleClass TDoubleClass;
struct _TDoubleClass {
    GObjectClass parent_class;
};

G_DEFINE_TYPE(TDouble,t_double,G_TYPE_OBJECT)

static void t_double_class_init(TDoubleClass* class){

}
static void t_double_init(TDouble *d){

}

typedef struct _Foo{
    int i;
    int j;
} Foo;

int main(){
  GType dtype;
  TDouble *d;

  TDouble b;
  Foo f;

  g_print("Size of TDouble: %d\n",sizeof(TDouble));
  g_print("Size of int: %d\n",sizeof(int));
  g_print("Size of double: %d\n",sizeof(double));
  g_print("Size of Foo: %d\n",sizeof(Foo));

  dtype = t_double_get_type();
  if (dtype)
    g_print("Registration was a success. The type is %lx.\n",dtype);
  else
    g_print("Registration failed.\n");

  d = g_object_new(T_TYPE_DOUBLE,NULL);
  if(d)
    g_print("Instantiation was a success. The instance address is %p",d);


}