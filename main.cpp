#include <iostream>
#include <vector>    // Libreria para crear y manipular vectores
#include <algorithm> // Utilizada para la funcion find, para recorrer todos los elementos de un vector
#include <stdlib.h>  // Para las funciones system: pause (Pausar consola} y cls (Limpipar consola}
#include <fstream>   // Utilizada pra la funcion ofstream para crear el txt en la funcion GuardarCashback
#include <conio.h>   // Utilizada para que al ingresar la clave se vean * en vez de lo que se escriba
#include <sstream>   // Utlizada en la funcion para construir el vector productos
#include <random>    // Libreria para generar un numero aleatorio par ael folio
#include <ctime>     // Libreria para las funciones donde se utilizan fechas y horas
using namespace std;

struct Producto
{
    string categoria;
    string nombre;
    string descripcion;
    double precio;
    int inventario;
};

vector<Producto> leerArchivoProductos(const string &nombreArchivo)
{
    vector<Producto> productos;
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open())
    {
        cout << "Error al abrir el archivo." << endl;
        return productos;
    }
    string linea;
    while (getline(archivo, linea)) // Extraer los datos de la linea y crear un objeto Producto
    {
        string categoria, nombre, descripcion;
        double precio;
        int inventario;
        stringstream ss(linea);
        getline(ss, categoria, ',');
        getline(ss, nombre, ',');
        getline(ss, descripcion, ',');
        ss >> precio;
        ss.ignore(); // Ignorar la coma despues del precio
        ss >> inventario;
        Producto producto{categoria, nombre, descripcion, precio, inventario}; // Crear el objeto Producto y agregarlo al vector
        productos.push_back(producto);
    }
    archivo.close();
    return productos;
}

vector<string> datosTarjeta;
vector<string> datosDireccion;
vector<Producto> carrito; // Declara el vector del carrito y construye el vector del vector productos con el txt
vector<Producto> productos = leerArchivoProductos("productos.txt");
vector<string> rangosHorarios = {
    {"8hrs - 10hrs"},
    {"10hrs - 12hrs"},
    {"12hrs - 14hrs"},
    {"14hrs - 16hrs"},
    {"16hrs - 18hrs"},
    {"18hrs - 20hrs"}};

double cashback;
string nombreUsuario, descuen, fechageneral, nip, rango;
int estu = 1;
double totalpagar;
double totalPagarDesc;
int cantidadProductos;
int folio; // Declara variables globales para poder ser utilizadas en otras funciones

void titulo(string title) // Funcion para decorar visualmente el titulo ingresado a la funcion
{
    int largotitulo = title.length() + 20;                                             // Calcula el largo total del titulo sumando 20 caracteres adicionales
    cout << string(largotitulo, '*') << "\n \n";                                       // Imprime una linea de asteriscos para separar visualmente el titulo
    largotitulo = largotitulo * 0.25;                                                  // Calcula el 25% del largo total del titulo
    cout << string(largotitulo, ' ') << title << string(largotitulo, ' ') << " \n \n"; // Imprime el titulo centrado con espacios en blanco a los lados
    largotitulo = largotitulo * 4;                                                     // Calcula el 100% del largo total del titulo
    cout << string(largotitulo, '*') << "\n \n";                                       // Imprime una linea de asteriscos para separar visualmente el titulo
}

int ObtenerNumeroValido(const string &mensaje, const string &mensajerror)
{                // Compurba que la entrada ingresada sea un numero positivo y valida que la entrada sea correcta
    int numero;  // Almacena el numero ingresado por el usuario
    while (true) // Ciclo while que se ejecutara hasta que se cumpla la condicion de salida
    {
        cout << mensaje;
        cin >> numero;                // Muestra el mensaje al usuario y solicita el numero
        if (cin.fail() || numero < 0) // Verifica si la entrada es invalida o si el numero ingresado es negativo
        {
            cin.clear();                 // Limpia el error al leer
            cin.ignore(10000, '\n');     // Ignora los caracteres restantes de entrada utilizando
            cout << mensajerror << endl; // Muestra un mensaje de error si la entrada es invalida
        }
        else // Si la entrada es valida
        {
            return numero; // Retorna el numero ingresado
        }
    }
}

void MostrarProductos()
{
    titulo("Productos disponibles");       // Imprime el titulo "Productos disponibles"
    int na = 1;                            // Variable para enumerar los productos
    vector<string> categorias;             // Vector para almacenar las categorias de los productos
    for (const auto &producto : productos) // Itera sobre todos los productos para obtener las categorias
    {
        if (find(categorias.begin(), categorias.end(), producto.categoria) == categorias.end())
        {
            categorias.emplace_back(producto.categoria); // Agrega la categoria al vector de categorias si no existe
        }
    }
    for (const auto &categoria : categorias) // Itera sobre las categorias y muestra los productos correspondientes
    {
        cout << categoria << ":\n";            // Imprime el nombre de la categoria
        for (const auto &producto : productos) // Itera sobre todos los productos y muestra los que pertenecen a la categoria actual
        {
            if (producto.categoria == categoria)
            {
                // Muestra la informacion del producto (nombre, descripcion, precio e inventario)
                cout << na << ".- " << producto.nombre << " - " << producto.descripcion << " ... $" << producto.precio << " (" << producto.inventario << " disponibles)\n";
                na++; // Incrementa el contador de productos
            }
        }
        cout << "\n"; // Imprime una linea en blanco para separar las categorias
    }
    cout << string(30, '*') << "\n\n"; // Imprime una linea de asteriscos para finalizar la funcion
}

void AgregarProducto(int seleccion)
{
    if (seleccion < 1 || seleccion > productos.size()) // Verifica si la seleccion esta dentro de los limites validos
    {
        cout << "\nOpcion invalida, por favor seleccione una opcion valida: \n\n";
        return;
    }
    Producto productoSeleccionado = productos[seleccion - 1]; // Obtener el producto seleccionado
    if (productoSeleccionado.inventario == 0)                 // Verifica si hay inventario disponible del producto seleccionado
    {
        cout << "\nNo hay mas inventario del producto seleccionado.\n\nPor favor seleccione algun otro producto.\n\n";
        system("pause");
        system("cls");
        MostrarProductos();
        return;
    }
    int cantidad = ObtenerNumeroValido("\nIngrese la cantidad de productos a agregar o 0 para cancelar: ", "\nCantidad invalida, porfavor ingrese una cantidad valida.\n"); // Pide la cantidad de productos a agregar
    if (cantidad == 0)                                                                                                                                                      // Verifica si se cancelo la operacion
    {
        system("cls");
        MostrarProductos();
        return;
    }
    if (cantidad > productoSeleccionado.inventario) // Verifica si la cantidad ingresada es mayor al inventario disponible
    {
        cout << "\nSolo hay " << productoSeleccionado.inventario << " unidades disponibles, por favor ingrese una cantidad menor o 0 para cancelar.\n";
        cantidad = ObtenerNumeroValido("\nIngrese la cantidad de productos a agregar o 0 para cancelar:", "\nCantidad invalida, porfavor ingrese una cantidad valida.\n");
    }
    for (int i = 0; i < cantidad; i++) // Agrega los productos seleccionados al carrito
    {
        carrito.emplace_back(productoSeleccionado);
    }
    for (auto it = productos.begin(); it != productos.end(); ++it) // Actualiza el inventario del producto seleccionado en el vector
    {
        if (it->nombre == productoSeleccionado.nombre)
        {
            it->inventario -= cantidad;
            break;
        }
    }
    // Actualiza el inventario del producto seleccionado en el archivo "productos.txt"
    ifstream archivoLectura("productos.txt");
    ofstream archivoTemporal("productos_temporal.txt");
    if (archivoLectura.is_open() && archivoTemporal.is_open())
    {
        string linea;
        while (getline(archivoLectura, linea))
        {
            stringstream ss(linea);
            string nombre, descripcion, categoria;
            double precio;
            int inventario;
            getline(ss, categoria, ',');
            getline(ss, nombre, ',');
            getline(ss, descripcion, ',');
            ss >> precio;
            ss.ignore();
            ss >> inventario;
            ss.ignore();
            if (nombre == productoSeleccionado.nombre)
            {
                inventario -= cantidad;
            }
            archivoTemporal << categoria << "," << nombre << "," << descripcion << "," << precio << "," << inventario << "\n";
        }
        archivoLectura.close();
        archivoTemporal.close();
        remove("productos.txt");
        rename("productos_temporal.txt", "productos.txt");
    }
    system("cls");      // Limpia la consola
    MostrarProductos(); // Vuelve a mostrar los productos
    if (cantidad == 1)  // Mostrar mensaje de confirmacion
    {
        cout << "Se agrego un articulo '" << productoSeleccionado.nombre << "' al carrito.\n\n";
    }
    else
    {
        cout << "El producto '" << productoSeleccionado.nombre << "' se agrego al carrito " << cantidad << " veces.\n\n";
    }
}

void MostrarCarrito()
{
    vector<string> productosEnCarrito;   // Almacena los nombres de los productos en el carrito
    vector<int> cantidadesEnCarrito;     // Almacena las cantidades de cada producto en el carrito
    vector<int> totalesPorProducto;      // Almacena los totales por producto en el carrito
    int i, j;                            // Variables para manejar los ciclos
    for (i = 0; i < carrito.size(); i++) // Recorre el carrito
    {
        bool productoExistente = false;
        for (j = 0; j < productosEnCarrito.size(); j++)
        {
            if (productosEnCarrito[j] == carrito[i].nombre) // Verifica si el producto ya esta en el carrito
            {
                cantidadesEnCarrito[j]++; // Incrementa la cantidad del producto en el carrito
                productoExistente = true; // Cambia la variable para que ya no se agregue nuevamente
                break;
            }
        }
        if (!productoExistente) // Si el producto no esta en el carrito lo agrega a los vectores
        {
            productosEnCarrito.push_back(carrito[i].nombre);
            cantidadesEnCarrito.push_back(1);
            totalesPorProducto.push_back(carrito[i].precio);
        }
        else
        {
            totalesPorProducto[j] += carrito[i].precio; // Actualizar el total por producto si el producto ya esta en el carrito
        }
    }
    for (int i = 0; i < productosEnCarrito.size(); i++) // Mouestra los productos en el carrito con sus cantidades y totales
    {
        cout << i + 1 << ".- '" << productosEnCarrito[i] << "' ($" << carrito[i].precio << ") _ Cantidad: " << cantidadesEnCarrito[i] << " ... Subotal: $" << totalesPorProducto[i] << "\n";
    }
    cout << "\n";
}

double GenerarCashback(double totalPagar)
{
    cashback = 0.0;                 // Inicia el cashback en 0
    cashback = (totalPagar * 0.05); // genera un cashback del 5% dependiendo del total a pagar
    return cashback;                // Regresa el valor del cashback
}

bool CantidadTotal()
{
    cantidadProductos = carrito.size(); // Almacena la cantidad de productos en el carrito
    double totalPagartemp = 0.0;        // Total a pagar
    system("cls");                      // Limpia la consola antes de mostrar el total
    if (carrito.empty())                // Verificar si el carrito esta vacio
    {
        titulo("Carrito de compras vacio");
        cout << "\nRegresando al menu principal...\n\n";
        system("pause");
        return true; // Regresa este valor para que ya no siga el demas proceso de la compra
    }
    else // Calcula el total a pagar sumando los precios de los productos en el carrito
    {
        for (const auto &producto : carrito) // Recorre cada uno de los productos en el carrito
        {
            totalPagartemp += producto.precio; // Se van sumando los precios de los productos al total a pagar
        }
        cout << "Carrito de compras: (" << cantidadProductos << " productos)" << endl; // Muestra la cantidad de productos que hay en el carrito
        MostrarCarrito();
        totalpagar = totalPagartemp;                              // Muesta los productos en el carrito
        cout << "\nTotal a pagar: $" << totalPagartemp << "\n\n"; // Mostrar el total a pagar
        return false;                                             // Regresa este valor para que siga el proceso de la compra
    }
}

void MostrarProductosInventario()
{
    productos = leerArchivoProductos("productos.txt");
    int inventarioTotal = 0; // Variable para almacenar el inventario total
    titulo("Inventario actual: ");
    vector<string> categorias;             // Vector para almacenar las categorias unicas
    for (const auto &producto : productos) // Obtiene las categorias unicas de los productos
    {
        if (find(categorias.begin(), categorias.end(), producto.categoria) == categorias.end()) // Verifica si la categoria ya esta en el vector de categorias
        {
            categorias.emplace_back(producto.categoria); // Agregar la categoria al vector de categorias
        }
    }
    for (const auto &categoria : categorias) // Recorre las categorias
    {
        int inventarioCategoria = 0;           // Variable para almacenar el inventario de la categoria
        cout << categoria << ":\n";            // Muesta la categoria
        for (const auto &producto : productos) // Recorre los productos y mostrar los detalles de cada producto de la categoria
        {
            if (producto.categoria == categoria)
            {
                cout << "* " << producto.nombre << "  (Inventario: " << producto.inventario << ")\n";
                inventarioCategoria += producto.inventario;
            }
        }
        cout << "\n";
        cout << "Inventario total de la categoria: " << categoria << " (" << inventarioCategoria << ") \n"; // Muestra el total de inventario por categoria
        cout << "\n";
        inventarioTotal += inventarioCategoria; // Actualizar el inventario total sumandole el inventario por categoria
    }
    cout << "Inventario total de todos los productos: " << inventarioTotal << "\n\n************************************\n\n"; // Muestra el inventario total de todos los productos
}

void mostrarCategorias(const vector<Producto> &productos)
{
    vector<string> categorias;
    for (const auto &producto : productos)
    {
        if (find(categorias.begin(), categorias.end(), producto.categoria) == categorias.end())
        {
            categorias.push_back(producto.categoria);
        }
    }
    titulo("Categorias existentes");
    for (const auto &categoria : categorias)
    {
        cout << "- " << categoria << "\n";
    }
    cout << "\n\n";
}

void ordenarArchivoPorCategoria(const string &Archivo)
{
    vector<Producto> productos = leerArchivoProductos(Archivo);
    // Ordenar los productos por categoria
    sort(productos.begin(), productos.end(), [](const Producto &a, const Producto &b)
         { return a.categoria < b.categoria; });
    // Escribir los productos ordenados en un archivo temporal
    ofstream archivoTemporal("productos_temporal.txt");
    if (archivoTemporal.is_open())
    {
        for (const Producto &producto : productos)
        {
            archivoTemporal << producto.categoria << "," << producto.nombre << "," << producto.descripcion << "," << producto.precio << "," << producto.inventario << "\n";
        }
        archivoTemporal.close();
    }
    // Reemplazar el archivo original con el archivo temporal
    remove(Archivo.c_str());
    rename("productos_temporal.txt", Archivo.c_str());
}

void RegistrarProducto(vector<Producto> &productos)
{
    string categoria, nombre, descripcion;
    double precio;
    int inventario;
    bool otro;
    while (otro)
    {
        system("cls");
        mostrarCategorias(productos);
        categoria.clear();
        nombre.clear();
        descripcion.clear();
        titulo("Ingrese los datos");
        cout << "(Confirmacion al final par cancelar)" << endl;
        cout << "\nIngresa la categoria o agregue una nueva: ";
        cin.ignore();
        getline(cin, categoria);
        cout << "\nIngresa el nombre: ";
        getline(cin, nombre);
        cout << "\nIngresa la descripcion: ";
        getline(cin, descripcion);
        precio = ObtenerNumeroValido("\nIngresa el precio: ", "\nNumero invalido, ingresa un numero valido:");
        inventario = ObtenerNumeroValido("\nIngresa el inventario a agregar: ", "\nCantidad de inventario no valida, ingrese un numero valido: ");
        bool confirmar;
        cout << "\n\n(Confirme los datos ingresados)" << endl;
        cout << "\nDesea agregar el producto ingresado? (s/n):";
        char ch;
        cin >> ch;
        while (cin.fail() || ch != 'n' && ch != 's') // Bucle while para validar la entrada del usuario
        {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "\nOpcion invalida, ingrese 's' o 'n': ";
            cin >> ch;
        }
        if (ch == 's')
        {
            confirmar = true;
            cout << "\nDesea agregar otro producto? (s/n): ";
            char ch1;
            cin >> ch1;
            while (cin.fail() || ch1 != 'n' && ch1 != 's') // Bucle while para validar la entrada del usuario
            {
                cin.clear();
                cin.ignore(100, '\n');
                cout << "\nOpcion invalida, ingrese 's' o 'n': ";
                cin >> ch1;
            }
            if (ch1 == 's')
            {
                otro = true;
            }
            else
            {
                otro = false;
            }
        }
        else
        {
            confirmar = false;
            cout << "\nDesea intentar agregar otro producto? (s/n): ";
            char otro;
            cin >> otro;
            while (cin.fail() || otro != 'n' && otro != 's') // Bucle while para validar la entrada del usuario
            {
                cin.clear();
                cin.ignore(100, '\n');
                cout << "\nOpcion invalida, ingrese 's' o 'n': ";
                cin >> otro;
            }
            if (otro == 's')
            {
                otro = true;
            }
            else
            {
                otro = false;
                break;
            }
        }
        if (confirmar)
        {
            ofstream archivo("productos.txt", ios::app);
            if (archivo.is_open())
            {
                archivo << categoria << "," << nombre << "," << descripcion << "," << precio << "," << inventario << "\n";
                archivo.close();
                cout << "\n\n";
                titulo("El producto se ha agregado correctamente");
            }
            vector<Producto> productosOrdenados = leerArchivoProductos("productos.txt");
            productos.clear();
            productos.insert(productos.end(), productosOrdenados.begin(), productosOrdenados.end());
            ordenarArchivoPorCategoria("productos.txt");
        }
    }
}

void modificarInventario()
{
    string nombreProducto;
    int nuevaCantidad = 1;
    while (nuevaCantidad != 0)
    {
        system("cls");
        MostrarProductosInventario();
        cout << "Para salir ingrese 0 en cantidad (Nada en nombre del producto)\n\n";
        cout << "\nIngrese el nombre del producto (Exactamente como estra escrito): ";
        cin.ignore();
        getline(cin, nombreProducto);
        cout << "\nIngrese la nueva cantidad de inventario o 0 para salir: ";
        cin >> nuevaCantidad;
        while (cin.fail() || nuevaCantidad < 0 || nuevaCantidad > 1000)
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nNumero invalido, porfavor ingresa un numero valido: ";
            cin >> nuevaCantidad;
        }
        if (nuevaCantidad == 0)
        {
            break;
        }
        if (nuevaCantidad != 0)
        {
            ifstream archivo("productos.txt"); // Abrir el archivo de texto
            // Leer los productos del archivo y guardarlos en un vector
            vector<Producto> productos;
            string linea;
            while (getline(archivo, linea))
            {
                Producto producto;
                stringstream ss(linea);
                getline(ss, producto.categoria, ',');
                getline(ss, producto.nombre, ',');
                getline(ss, producto.descripcion, ',');
                ss >> producto.precio;
                ss.ignore();
                ss >> producto.inventario;
                productos.push_back(producto);
            }
            // Buscar el producto por nombre y modificar su inventario
            for (auto &producto : productos)
            {
                if (producto.nombre == nombreProducto)
                {
                    producto.inventario = nuevaCantidad;
                    break;
                }
            }
            // Cerrar el archivo de texto
            archivo.close();
            // Abrir el archivo en modo de escritura
            ofstream archivoSalida("productos.txt");
            if (!archivoSalida)
            {
                cout << "No se pudo abrir el archivo de salida." << endl;
                return;
            }
            // Escribir los productos actualizados en el archivo
            for (const auto &producto : productos)
            {
                archivoSalida << producto.categoria << "," << producto.nombre << "," << producto.descripcion << "," << producto.precio << "," << producto.inventario << endl;
            }
            // Cerrar el archivo de salida
            archivoSalida.close();
        }
    }
}

int ObtenerEstu()
{
    FILE *users;                             // Se declara un puntero al archivo de usuarios
    users = fopen("users.txt", "r");         // Abre el archivo en modo de lectura
    char linea[100];                         // Variable para almacenar cada linea del archivo
    bool encontrado = false;                 // Variable para indicar si se encontro el usuario
    while (fgets(linea, 100, users) != NULL) // Lee cada linea del archivo
    {
        char nombretemp[50];
        char niptemp[50];
        double temp3;
        int estuTemp;                                                          // Se declaran variables temporales para almacenar los valores de cada usuarioescaneado
        sscanf(linea, "%s %s %lf %d", nombretemp, niptemp, &temp3, &estuTemp); // Escanea los valores de la linea y los almacena en las variables temporales
        if (nombreUsuario == nombretemp && nip == niptemp)                     // Comprueba si el nombre y el NIP coinciden con los valores globales actuales
        {
            encontrado = true; // Indica que se encontro el usuario
            estu = estuTemp;   // Asigna el valor de estuTemp a la variable global estu
            break;             // Rompe el ciclo para dejar de escanear el archivo
        }
    }
    fclose(users); // Cierra el archivo
    if (encontrado)
    {
        return estu; // Devuelve el valor de estu obtenido
    }
}

void GuardarCashback()
{
    bool encontrado = false; // Variable para controlar si se encontro el usuario
    while (!encontrado)      // Bucle while que se ejecuta hasta que se encuentre el usuario o se decida no volver a intentar
    {
        string nombretemp;
        string niptemp;
        int estutemp;
        double cashbackusuario; // Se declaran variables temporales para almacenar los datos de los usuarios
        ofstream tempFile;      // Objeto ofstream para escribir en un archivo temporal
        cout << "Ingrese su nombre de cliente: ";
        cin >> nombreUsuario; // Se solicita el nombre de usuario
        cout << "\nIngrese su nip: ";
        char ch;
        nip = "";
        while ((ch = getch()) != '\r') // Lee cada caracter hasta que se presione Enter
        {
            if (ch == '\b') // Si se presiona la tecla de retroceso
            {
                if (!nip.empty())
                {
                    nip.pop_back();  // Eliminar el ultimo caracter ingresado
                    cout << "\b \b"; // Borrar el ultimo asterisco impreso
                }
            }
            else
            {
                nip.push_back(ch); // Agregar el caracter ingresado a la cadena nip
                cout << "*";       // Imprimir un asterisco en lugar del caracter ingresado
            }
        }                                                                     // Se solicita el su NIP
        ifstream users("users.txt");                                          // Objeto ifstream para leer el archivo de usuarios
        tempFile.open("temp.txt");                                            // Se abre el archivo temporal para escribir
        bool usuarioEncontrado = false;                                       // Variable para controlar si se encuentro el usuario
        while (users >> nombretemp >> niptemp >> cashbackusuario >> estutemp) // Bucle while que lee los datos de cada usuario
        {
            if (nombreUsuario == nombretemp && nip == niptemp) // Comprueba si el nombre y el NIP ingresados coinciden con los del usuario actual
            {
                cashbackusuario += cashback;                                                                   // Incrementa el cashback del usuario actual
                tempFile << nombretemp << " " << niptemp << " " << cashbackusuario << " " << estutemp << endl; // Escribe los datos actualizados en el archivo temporal
                cout << "\n\n";
                titulo("Cashback guardado exitosamente");
                system("pause");
                usuarioEncontrado = true; // Actualiza la variable "usuarioEncontrado" a true para indicar que se encontro el usuario
                encontrado = true;        // Actualiza la variable "encontrado" a true para salir del bucle
            }
            else
            {
                tempFile << nombretemp << " " << niptemp << " " << cashbackusuario << " " << estutemp << endl; // Escribe los datos del usuario actual en el archivo temporal sin cambios
            }
        }
        users.close();         // Cierra el archivo de usuarios
        tempFile.close();      // Cierra el archivo temporal
        if (usuarioEncontrado) // Si se encontro el usuario en el archivo
        {
            remove("users.txt");             // Elimina el archivo de usuarios original
            rename("temp.txt", "users.txt"); // Renombra el archivo temporal como el archivo de usuarios original
        }
        else // Si no se encontro el usuario en el archivo
        {
            cout << "\nUsuario no encontrado o nip no valido" << endl;
            char opcion;
            cout << "\n\nDesea volver a intentar? (s/n): ";
            cin >> opcion;
            while (cin.fail() || opcion != 'n' && opcion != 's') // Bucle while para validar la entrada del usuario
            {
                cin.clear();
                cin.ignore(10, '\n');
                cout << "\nOpcion invalida, ingrese 's' o 'n': ";
                cin >> opcion;
            }
            if (opcion == 's') // Si el usuario desea volver a intentar
            {
                system("cls"); // Limpia la pantalla
            }
            else // Si el usuario no desea volver a intentar
            {
                nip.clear();
                descuen = "0%";
                nombreUsuario = "No cliente";
                estu = 1;
                cashback = 0.0;
                totalPagarDesc = totalpagar;
                break; // Sale del bucle while principal
            }
        }
    }
}

void RegistrarUsuario()
{
    bool continuar;
    while (continuar)
    {
        nip.clear();
        nombreUsuario.clear(); // Limpia las variables globales
        string niptemp;        // Variable temporal para confirmar el nip
        bool nipvalido;        // Variable para verificar si el nip es valido
        while (nipvalido)
        {
            system("cls"); // Limpia la consola
            cout << "Ingrese un nombre de cliente: ";
            cin >> nombreUsuario; // Pide el nombre del usuario
            cout << "\nIngrese un nip (5 digitos): ";
            char ch;
            nip = "";
            while ((ch = getch()) != '\r') // Lee cada caracter hasta que se presione Enter
            {
                if (ch == '\b') // Si se presiona la tecla de retroceso
                {
                    if (!nip.empty())
                    {
                        nip.pop_back();  // Eliminar el ultimo caracter ingresado
                        cout << "\b \b"; // Borrar el ultimo asterisco impreso
                    }
                }
                else
                {
                    nip.push_back(ch); // Agregar el caracter ingresado a la cadena nip
                    cout << "*";       // Imprimir un asterisco en lugar del caracter ingresado
                }
            }
            while (nip.length() != 5) // Verifica que el nip tenga 5 digitos
            {
                cout << "\nNip invalido, por favor ingrese un nip de 5 digitos: ";
                nip.clear();
                char ch;
                nip = "";
                while ((ch = getch()) != '\r')
                {
                    if (ch == '\b')
                    {
                        if (!nip.empty())
                        {
                            nip.pop_back();
                            cout << "\b \b";
                        }
                    }
                    else
                    {
                        nip.push_back(ch);
                        cout << "*";
                    }
                }
            }
            cout << "\n\nConfirme su nip: ";
            char ch1;
            niptemp = "";
            while ((ch1 = getch()) != '\r')
            {
                if (ch1 == '\b')
                {
                    if (!niptemp.empty())
                    {
                        niptemp.pop_back();
                        cout << "\b \b";
                    }
                }
                else
                {
                    niptemp.push_back(ch1);
                    cout << "*";
                }
            }
            if (nip != niptemp) // Verificar si el nip y la confirmacion coinciden
            {
                cout << "\n\nNip no coincide\n\n";
                cout << "Desea volver a intentar? (s/n): ";
                char opcion;
                cin >> opcion;
                while (cin.fail() || opcion != 'n' && opcion != 's') // Bucle while para validar la entrada del usuario
                {
                    cin.clear();
                    cin.ignore(10, '\n');
                    cout << "\nOpcion invalida, ingrese 's' o 'n': ";
                    cin >> opcion;
                }
                if (opcion == 's') // Si el usuario desea volver a intentar
                {
                    nombreUsuario.clear();
                    nip.clear();
                    niptemp.clear();
                    system("csl");
                }
                else // Si el usuario no desea volver a intentar
                {
                    estu = 1;
                    cashback = 0.0;
                    nombreUsuario = "No cliente";
                    continuar = false;
                    break; // Sale del bucle while principal
                }
            }
            else
            {
                niptemp.clear();
                nipvalido = false; // El nip es valido, salir del bucle
                break;
            }
        };
        if (continuar)
        {
            cout << "\n\nEs estudiante?\n1 = No estudiante\n2 = Estudiante general\n3 = Estudiante BUAP\n\nSeleccione la opcion: ";
            cin >> estu;                               // Captura el valor de si es estudiante o no
            while (cin.fail() || estu < 1 || estu > 3) // Verificar que la opcion sea valida
            {
                cin.clear();
                cin.ignore(256, '\n');
                cout << "Opcion invalida, por favor ingrese una opcion valida: ";
                cin >> estu;
            }
            FILE *users;
            users = fopen("users.txt", "r");
            char nombretemp[50];
            string temp1;
            double temp2;
            int temp3;
            bool encontrado = false;
            while (fscanf(users, "%s %s %.2f %d\n", nombretemp, &temp1, &temp2, &temp3) != EOF) // Leer los datos de usuarios existentes
            {
                if (nombreUsuario == nombretemp) // Verificar si el nombre de usuario ya existe
                {
                    encontrado = true;
                    continuar = false;
                    cout << endl;
                    titulo("El cliente ya esta registrado");
                    cout << "\nGuardando cashback en su usuario...\n\n";
                    system("pause");
                    system("cls");
                    nip = "";
                    nombreUsuario.clear();
                    GuardarCashback();
                    break;
                }
            }
            if (!encontrado) // Si el usuario no esta registrado, agregarlo al archivo
            {
                users = fopen("users.txt", "a");
                fprintf(users, "%s %s %.2f %d\n", nombreUsuario.c_str(), nip.c_str(), cashback, estu);
                fclose(users);
                cout << "\n\n";
                titulo("Cliente registrado exitosamente");
                system("pause");
                break;
            }
            fclose(users);
        }
    }
}

void calcularDesc()
{
    estu = ObtenerEstu();
    system("cls");
    double desc;
    switch (estu)
    {
    case 1:
        desc = 1;
        descuen = "0%";
        break;
    case 2:
        desc = 0.95;
        descuen = "5%";
        break;
    case 3:
        desc = 0.85;
        descuen = "15%";
        break;
    default:
        break;
    }
    totalPagarDesc = (totalpagar * desc);
    cout << "Carrito de compras: (" << cantidadProductos << " productos)" << endl; // Muestra la cantidad de productos que hay en el carrito
    MostrarCarrito();
    cout << "\nTotal: $" << totalpagar << "\nDescuento: " << descuen << "\nTotal a pagar: $" << totalPagarDesc << "\nCashback acumulado: $" << cashback << "\n\n";
    system("pause");
}

vector<string> ingresarDatosTarjeta()
{
    system("cls");
    titulo("Datos de pago");
    cout << "\n";
    vector<string> datosTarjeta;
    string numeroTarjeta;
    cout << "Ingrese el numero de tarjeta: ";
    cin >> numeroTarjeta;
    while (numeroTarjeta.length() != 16 || cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nNumero invalido, ingrese un numero de tarjeta valido (16 digitos): ";
        cin >> numeroTarjeta;
    }
    datosTarjeta.push_back(numeroTarjeta);
    string nombreTitular;
    cout << "\nIngrese el nombre del titular: ";
    cin.ignore();
    getline(cin, nombreTitular);
    datosTarjeta.push_back(nombreTitular);
    string fechaVencimiento;
    cout << "\nIngrese la fecha de vencimiento: ";
    cin >> fechaVencimiento;
    if (fechaVencimiento.length() != 5 || fechaVencimiento[2] != '/')
    {
        cout << "\nFecha incorrecta, por favor ingrese una fecha valida en formato correcto (00/00):";
        cin >> fechaVencimiento;
    }
    datosTarjeta.push_back(fechaVencimiento);
    string codigoSeguridad;
    cout << "\nIngrese el codigo de seguridad: ";
    char ch;
    codigoSeguridad = "";
    while ((ch = getch()) != '\r') // Lee cada caracter hasta que se presione Enter
    {
        if (ch == '\b') // Si se presiona la tecla de retroceso
        {
            if (!nip.empty())
            {
                codigoSeguridad.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            codigoSeguridad.push_back(ch);
            cout << "*";
        }
    }
    while (codigoSeguridad.length() != 3 || cin.fail())
    {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\n\nCodigo de seguridad invalido, ingrese un codigo de seguridad valido (3 digitos): ";
        codigoSeguridad = "";
        while ((ch = getch()) != '\r') // Lee cada caracter hasta que se presione Enter
        {
            if (ch == '\b') // Si se presiona la tecla de retroceso
            {
                if (!nip.empty())
                {
                    codigoSeguridad.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                codigoSeguridad.push_back(ch);
                cout << "*";
            }
        }
    }
    return datosTarjeta;
}

void imprimirDatosTarjeta(const vector<string> &datosTarjeta)
{
    cout << "\n\n";
    titulo("Confirmacion de datos de pago");
    cout << "Numero de tarjeta: " << datosTarjeta[0] << endl;
    cout << "\nNombre del titular: " << datosTarjeta[1] << endl;
    cout << "\nFecha de vencimiento: " << datosTarjeta[2] << endl;
    cout << "\nCodigo de seguridad: ***\n\n";
}

vector<string> ingresarDatosDireccion()
{
    titulo("Datos de envio");
    vector<string> datosDireccion;
    string calle;
    cout << "Ingrese el nombre de la calle: ";
    cin.ignore();
    getline(cin, calle);
    datosDireccion.push_back(calle);
    string numero;
    cout << "\nNumero exteriror: ";
    cin >> numero;
    while (cin.fail() || numero.length() > 6)
    {
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\nNumero invalido, por faovr ingrese un numero valido: ";
        cin >> numero;
    }
    datosDireccion.push_back(numero);
    string colonia;
    cout << "\nIngrese el nombre de la colonia: ";
    cin.ignore();
    getline(cin, colonia);
    datosDireccion.push_back(colonia);
    string codigoPostal;
    cout << "\nIngrese el codigo postal: ";
    cin >> codigoPostal;
    while (codigoPostal.length() != 5 || cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nCodigo postal invalido, ingrese un CP valido (5 digitos): ";
        cin >> codigoPostal;
    }
    datosDireccion.push_back(codigoPostal);
    return datosDireccion;
}

void imprimirDatosDireccion(const vector<string> &datosDireccion)
{
    cout << "\n\n";
    titulo("Confirmacion de datos de direccion");
    cout << "Calle: " << datosDireccion[0] << endl;
    cout << "\nNumero: #" << datosDireccion[1] << endl;
    cout << "\nColonia : " << datosDireccion[2] << endl;
    cout << "\nCodigo Postal: " << datosDireccion[3] << "\n\n";
}

string obtenerFechaHoy()
{
    time_t tiempoActual = time(0);
    tm *fechaActual = localtime(&tiempoActual);
    int diaActual = fechaActual->tm_mday;
    int mesActual = fechaActual->tm_mon + 1;
    string fechaHoy = to_string(diaActual) + "/" + to_string(mesActual);
    return fechaHoy;
}

string obtenerFechasSuiguiente()
{
    time_t tiempoActual = time(0);
    tm *fechaHoraActual = localtime(&tiempoActual);
    int diasSuiguiente = fechaHoraActual->tm_mday + 1;
    int messSuiguiente = fechaHoraActual->tm_mon + 1;
    string fechasSuiguiente = to_string(diasSuiguiente) + "/" + to_string(messSuiguiente);
    return fechasSuiguiente;
}

string ingresarFechaPersonalizada()
{
    string fechaPersonalizada;
    bool formatoValido = false;
    while (!formatoValido)
    {
        cout << "\nIngrese una fecha en formato dia/mes: ";
        cin >> fechaPersonalizada;
        if (fechaPersonalizada.length() == 5 && fechaPersonalizada[2] == '/')
        {
            int dia = stoi(fechaPersonalizada.substr(0, 2));
            int mes = stoi(fechaPersonalizada.substr(3, 2));
            if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12)
            {
                formatoValido = true;
            }
        }
        if (!formatoValido)
        {
            cout << "Formato de fecha incorrecto, por favor ingrese una fecha valida en formato correcto (00/00)\n";
            fechaPersonalizada.clear();
        }
    }
    return fechaPersonalizada;
}

int generarFolio()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100000, 999999);
    return dis(gen);
}

int obtenerHoraActual()
{
    time_t tiempoActual = time(0);
    tm *fechaHoraActual = localtime(&tiempoActual);
    int horaActual = fechaHoraActual->tm_hour;
    return horaActual;
}

int obtenerHoraFin(const string &rangoHorario)
{
    int posGuion = rangoHorario.find("-");
    string horaFinStr = rangoHorario.substr(posGuion + 2, 2);
    int horaFin = stoi(horaFinStr);
    return horaFin;
}

string seleccionarRangoHorario(const vector<string> &rangosHorarios)
{
    int seleccion;
    while (true)
    {
        system("cls");
        titulo("Horarios disponibles");
        for (int i = 0; i < rangosHorarios.size(); i++)
        {
            cout << i + 1 << ". " << rangosHorarios[i] << endl;
        }
        cout << "\nSeleccione un rango horario: ";
        cin >> seleccion;
        if (seleccion >= 1 && seleccion <= rangosHorarios.size() || !cin.fail())
        {
            string rangoSeleccionado = rangosHorarios[seleccion - 1];
            int horaActual = obtenerHoraActual();
            int horaFin = obtenerHoraFin(rangoSeleccionado);
            if (horaActual <= horaFin)
            {
                return rangoSeleccionado;
            }
            else
            {
                cout << "\nEl rango horario seleccionado ya no esta disponible. Por favor, elija otro rango." << endl;
                system("pause");
            }
        }
        else
        {
            cout << "Seleccion invalida. Por favor, seleccione un numero valido de rango horario." << endl;
            system("pause");
        }
    }
}

string seleccionarRangoHorariofacil(const vector<string> &rangosHorarios)
{
    int seleccion;
    while (true)
    {
        system("cls");
        titulo("Horarios disponibles");
        for (int i = 0; i < rangosHorarios.size(); i++)
        {
            cout << i + 1 << ". " << rangosHorarios[i] << endl;
        }
        cout << "\nSeleccione un rango horario: ";
        cin >> seleccion;
        if (seleccion >= 1 && seleccion <= rangosHorarios.size() || !cin.fail())
        {
            string rangoSeleccionado = rangosHorarios[seleccion - 1];
            return rangoSeleccionado;
        }
        else
        {
            cout << "Seleccion invalida. Por favor, seleccione un numero valido de rango horario." << endl;
            system("pause");
        }
    }
}

void entrega()
{
    system("cls");
    string fechahoy = obtenerFechaHoy();
    string fechasiguiente = obtenerFechasSuiguiente();
    titulo("Seleccione un dia");
    cout << "\n1.  " << fechahoy << "\n2.  " << fechasiguiente << "\n3.  Otra fecha\n\nSelecione una opcion:";
    int escojerfecha;
    cin >> escojerfecha;
    while (cin.fail() || escojerfecha != 1 && escojerfecha != 2 && escojerfecha != 3)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nOpcion invalida, seleccione una opcion valida: ";
        cin >> escojerfecha;
    }
    switch (escojerfecha)
    {
    case 1:
    {
        fechageneral = fechahoy;
        rango = seleccionarRangoHorario(rangosHorarios);
        system("cls");
        titulo("Fecha asignada");
        cout << "Dia seleccionado: " << fechahoy << endl;
        cout << "\nHora seleccionada: " << rango << "\n\n";
        folio = generarFolio();
        system("pause");
    }
    break;
    case 2:
    {
        fechageneral = fechasiguiente;
        rango = seleccionarRangoHorariofacil(rangosHorarios);
        system("cls");
        titulo("Fecha asignada");
        cout << "Dia seleccionado: " << fechasiguiente << endl;
        cout << "\nHora seleccionada: " << rango << "\n\n";
        folio = generarFolio();
        system("pause");
    }
    break;
    case 3:
    {
        string fecha = ingresarFechaPersonalizada();
        if (!fecha.empty())
        {
            cout << "\nFecha ingresada: " << fecha << "\n\n";
        }
        system("pause");
        fechageneral = fecha;
        rango = seleccionarRangoHorariofacil(rangosHorarios);
        system("cls");
        titulo("Fecha asignada");
        cout << "Dia seleccionado: " << fechasiguiente << endl;
        cout << "\nHora seleccionada: " << rango;
        folio = generarFolio();
        system("pause");
    }
    break;
    default:
        break;
    }
}

void confirmacion()
{
    system("cls");
    titulo("Confirmacion de pedido");
    cout << "Carrito de compras: (" << cantidadProductos << " productos)" << endl; // Muestra la cantidad de productos que hay en el carrito
    MostrarCarrito();
    cout << "\nTotal: $" << totalpagar << "\nDescuento: " << descuen << "\nTotal a pagar: $" << totalPagarDesc << "\nCashback acumulado: $" << cashback << "\n\nCliente: " << nombreUsuario << "\n\nFolio de entrega: " << folio;
    imprimirDatosTarjeta(datosTarjeta);
    cout << "\n\nHora asignada: " << rango;
    cout << "\n\nDia asignado: " << fechageneral << "\n\n";
    system("pause");
}

void confirmacionDomicilo()
{
    system("cls");
    titulo("Confirmacion de pedido");
    cout << "Carrito de compras: (" << cantidadProductos << " productos)" << endl; // Muestra la cantidad de productos que hay en el carrito
    MostrarCarrito();
    cout << "\nTotal: $" << totalpagar << "\nDescuento: " << descuen << "\nTotal a pagar: $" << totalPagarDesc << "\nCashback acumulado: $" << cashback << "\n\nCliente: " << nombreUsuario << "\n\nFolio de entrega: " << folio;
    imprimirDatosTarjeta(datosTarjeta);
    cout << "\n\nHora asignada: " << rango;
    cout << "\n\nDia asignado: " << fechageneral << "\n\n";
    imprimirDatosDireccion(datosDireccion);
    system("pause");
}

void Pagar()
{
    system("cls");
    titulo("Seleccione su metodo de entrega");
    int seleccion;
    cout << "\n1. Recoger en tienda\n2. Entrega a domicilio\n0. Cancelar compra\n\nSeleccione una opcion: ";
    cin >> seleccion;
    while (cin.fail() || seleccion != 1 && seleccion != 2 && seleccion != 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\nOpcion invalida, selecione una opcion valida: ";
        cin >> seleccion;
    }
    if (seleccion == 0)
    {
        return;
    }
    switch (seleccion)
    {
    case 1:
    {
        entrega();
        system("cls");
        datosTarjeta = ingresarDatosTarjeta();
        system("cls");
        imprimirDatosTarjeta(datosTarjeta);
        system("pause");
        confirmacion();
    }
    break;
    case 2:
    {
        entrega();
        system("cls");
        datosDireccion = ingresarDatosDireccion();
        system("cls");
        imprimirDatosDireccion(datosDireccion);
        system("pause");
        system("cls");
        datosTarjeta = ingresarDatosTarjeta();
        system("cls");
        imprimirDatosTarjeta(datosTarjeta);
        system("pause");
        system("cls");
        confirmacionDomicilo();
    }
    break;
    default:
        break;
    }
}

void VerUsuarios()
{
    FILE *users;                     // Puntero a archivo para leer el archivo de usuarios
    users = fopen("users.txt", "r"); // Abre el archivo de usuarios en modo de lectura
    char nombretemp[50];
    string niptemp;
    double cashbacktemp;
    int estudi;
    string estudien; // Se declaran variables temporales para almacenar los datos de los usuarios
    cout << "Clientes registrados: \n\n";
    while (fscanf(users, "%s %s %lf %d", &nombretemp, &niptemp, &cashbacktemp, &estudi) != EOF) // Lee los datos de cada usuario del archivo
    {
        if (estudi == 1)
        {
            estudien = "No estudiante";
        }
        else if (estudi == 2)
        {
            estudien = "Estudiante";
        }
        else if (estu == 3)
        {
            estudien = "Estudiante BUAP";
        }
        cout << "Nombre de cliente: " << nombretemp << endl;
        cout << "Cashback: $" << cashbacktemp << endl;
        cout << "Estudiante: " << estudien << "\n\n";
    }
    fclose(users); // Cierra el archivo de usuarios
}

string LeerClave()
{
    FILE *archivo;
    archivo = fopen("clave.txt", "r");
    char clave[100];
    fscanf(archivo, "%s", clave);
    fclose(archivo);
    return string(clave);
}

void ModificarClave()
{
    string claveActual = LeerClave();
    string pedclave;
    cout << "Ingrese la clave actual: ";
    char ch;
    while ((ch = getch()) != '\r')
    {
        if (ch == '\b')
        {
            if (!pedclave.empty())
            {
                pedclave.pop_back();
                cout << "\b \b";
            }
        }
        else
        {
            pedclave.push_back(ch);
            cout << "*";
        }
    }
    if (pedclave == claveActual)
    {
        pedclave.clear();
        cout << "\n\nIngrese la nueva clave: ";
        string nuevaClave;
        char ch2;
        while ((ch2 = getch()) != '\r')
        {
            if (ch2 == '\b')
            {
                if (!nuevaClave.empty())
                {
                    nuevaClave.pop_back();
                    cout << "\b \b";
                }
            }
            else
            {
                nuevaClave.push_back(ch2);
                cout << "*";
            }
        }
        FILE *archivo;
        archivo = fopen("clave.txt", "r+");
        fprintf(archivo, "%s", nuevaClave.c_str());
        fclose(archivo);
        cout << "\n\nClave modificada exitosamente.\n\n";
    }
    else
    {
        pedclave.clear();
        cout << "\n\nClave incorrecta.\n\n";
    }
    system("pause");
}

void Cliente()
{
    cashback = 0.0;
    titulo("---TIENDA EN LINEA---");
    system("pause");
    system("cls");
    MostrarProductos();
    int seleccion, client;
    bool compraFinalizada = false; // Varaible para controlar si la compra esta finalizada
    while (!compraFinalizada)
    {
        int seleccion = ObtenerNumeroValido("Ingrese el numero del producto para agregar al carrito o 0 para finalizar la compra: ", "\nOpcion invalida, ingrese una opcion correcta\n\n");
        if (seleccion == 0)
        {
            compraFinalizada = true; // Cambiar la variable para finalizar la compra
        }
        else
        {
            AgregarProducto(seleccion);
        }
    }
    if (CantidadTotal())
    {
        return; // Sale de la funcion si la compra esta vacia
    }
    cout << "\nCuenta con un usuario para guardar cashback?\n1. Si\n2. No\n\nSeleccione una opcion: ";
    cin >> client;
    while (cin.fail() || (client != 1 && client != 2))
    {
        cout << "\nOpcion invalida\nIngrese una opcion valida (Valor numerico de la opcion): ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> client;
    }
    switch (client)
    {
    case 1:
        system("cls");
        cashback = GenerarCashback(totalpagar);
        GuardarCashback();
        calcularDesc();
        Pagar();
        break;
    case 2:
        cout << "\nDesea resgistrarse como cliente nuevo?\n1. Si\n2. No\n\nSeleccione una opcion: ";
        int regis;
        cin >> regis;
        while (cin.fail() || regis != 1 && regis != 2)
        {
            cout << "\nOpcion invalida\nIngrese una opcion valida (Valor numerico de la opcion1): ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> regis;
        }
        switch (regis)
        {
        case 1:
            cashback = GenerarCashback(totalpagar);
            RegistrarUsuario();
            calcularDesc();
            Pagar();
            break;
        case 2:
        {
            system("cls");
            CantidadTotal();
            system("pause");
            Pagar();
            descuen = "0%";
            nombreUsuario = "No cliente";
            estu = 1;
            cashback = 0.0;
            totalPagarDesc = totalpagar;
        }
        break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    carrito.clear();
}

void menuEmpleado()
{
    bool salirMenu = false;
    while (!salirMenu)
    {
        system("cls");
        titulo("Bienvenido empleados");
        cout << "1. Ver inventario de los productos\n2. Reabastecer el inventario\n3. Agregar producto\n4. Ver clientes\n5. Modificar clave de acceso\n0. Volver\n\nQue desea hacer?: ";
        int ele2;
        cin >> ele2;
        while (cin.fail() || (ele2 != 0 && ele2 != 1 && ele2 != 2 && ele2 != 3 && ele2 != 4 && ele2 != 5))
        {
            cout << "\nOpcion invalida\nIngrese una opcion valida (Valor numerico de la opcion): ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> ele2;
        }
        if (ele2 == 0)
        {
            system("cls");
            salirMenu = true;
            break;
        }
        switch (ele2)
        {
        case 1:
            system("cls");
            MostrarProductosInventario();
            system("pause");
            break;
        case 2:
            modificarInventario();
            break;
        case 3:
            RegistrarProducto(productos);
            system("pause");
            break;
        case 4:
            system("cls");
            VerUsuarios();
            system("pause");
            break;
        case 5:
            system("cls");
            ModificarClave();
            break;
        default:
            break;
        }
    }
}

int main()
{
    int ele;
    while (true)
    {
        productos = leerArchivoProductos("productos.txt");
        string clave = LeerClave();
        string pedclave;
        system("cls");
        titulo("Bienvenido a lobo tienda");
        cout << "Como desea ingresar:\n\n1. Cliente\n2. Empleado\n0. Salir\n\nSeleccione una opcion: ";
        cin >> ele;
        while (cin.fail() || (ele != 0 && ele != 1 && ele != 2))
        {
            cout << "\nOpcion invalida\nIngrese una opcion valida (Valor numerico de la opcion): ";
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> ele;
        }
        if (ele == 0)
        {
            break;
        }
        switch (ele)
        {
        case 1:
            system("cls");
            Cliente();
            break;
        case 2:
            cout << "\nIngrese la clave: ";
            char ch;
            while ((ch = getch()) != '\r') // Leer cada caracter hasta que se presione Enter
            {
                if (ch == '\b') // Si se presiona la tecla de retroceso
                {
                    if (!pedclave.empty())
                    {
                        pedclave.pop_back(); // Eliminar el ultimo caracter ingresado
                        cout << "\b \b";     // Borrar el ultimo asterisco impreso
                    }
                }
                else
                {
                    pedclave.push_back(ch); // Agregar el caracter ingresado a la cadena pedclave
                    cout << "*";            // Imprimir un asterisco en lugar del caracter ingresado
                }
            }
            if (pedclave == clave)
            {
                pedclave.clear();
                menuEmpleado();
            }
            else
            {
                pedclave.clear();
                cout << "\nClave invalida.\n\n";
                system("pause");
            }
            break;
        default:
            break;
        }
    }
    system("cls");
    titulo("Vuelva pronto");
    system("pause");
    return 0;
}
