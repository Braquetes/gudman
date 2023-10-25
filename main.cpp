#include <iostream>
#include <vector>    // Libreria para crear y manipular vectores
#include <algorithm> // Utilizada para la funcion find, para recorrer todos los elementos de un vector
#include <stdlib.h>  //Para las funciones system: pause (Pausar consola) y cls (Limpipar consola)
#include <fstream>   //Utilizada pra la funcion ofstream para crear el txt en la funcion guardarCashback
#include <string>
#include <cmath>
#include <limits>
#include <cstdio>

using namespace std;
void titulo(string title)
{
    int si = title.length() + 20;
    cout << string(si, '*') << "\n \n";
    si = si * 0.25;
    cout << string(si, ' ') << title << string(si, ' ') << " \n \n";
    si = si * 4;
    cout << string(si, '*') << "\n \n";
}

class Producto // Estructura de las propiedades que tienen los productos y delarara las variables con su mismo nombre de las carecteristicas
{
public:
    string nombre;
    string categoria;
    string descripcion;
    double precio;
    int inventario;
    Producto(string categoria, string nombre, string descripcion, int precio, int inventario) : categoria(categoria), nombre(nombre), descripcion(descripcion), precio(precio), inventario(inventario) {}
};

vector<Producto> carrito; // Declara los vectores de los productos y declara el carrito
vector<Producto> productos = {
    Producto("Lacteos", "Leche", "1L", 30.50, 24),
    Producto("Lacteos", "Quesillo", "250g", 38, 10),
    Producto("Lacteos", "Crema", "200ml", 20, 12),
    Producto("Lacteos", "Mantequilla", "245g", 40.50, 12),
    Producto("Lacteos", "Helado", "1.3L", 94, 6),
    Producto("Lacteos", "Margarina", "500g", 70, 12),
    Producto("Frutas y Verduras", "Manzanas", "1Kg", 54, 6),
    Producto("Frutas y Verduras", "Naranja", "1Kg", 29.90, 6),
    Producto("Frutas y Verduras", "Platano", "1Kg", 20, 6),
    Producto("Frutas y Verduras", "Jitomate", "1Kg", 29.90, 6),
    Producto("Frutas y Verduras", "Chile", "100g", 6, 30),
    Producto("Frutas y Verduras", "Lechuga", "1pz", 28, 8),
    Producto("Panaderia", "Donas", "6pz", 45, 5),
    Producto("Panaderia", "Telera", "1pz", 2.20, 20),
    Producto("Panaderia", "Cuerno", "1pz", 8, 14),
    Producto("Panaderia", "Concha", "1pz", 6, 16),
    Producto("Panaderia", "Pan de muerto", "1pz", 15, 10),
    Producto("Panaderia", "Pastel", "1pz", 300, 2),
    Producto("Farmacia", "Paracetamol (500mg)", "24tbl", 35, 12),
    Producto("Farmacia", "Ibuprufeno (600mg)", "10tbl", 50, 12),
    Producto("Farmacia", "Jeringas (3ml)", "5pz", 20, 10),
    Producto("Farmacia", "Pruebas de embarazo", "Generica", 75, 6),
    Producto("Farmacia", "Alcohol etilico (70)", "500ml", 45.50, 12),
    Producto("Farmacia", "Condones", "3pz", 45, 10),
    Producto("Higiene personal", "Crema corporal", "400ml", 92, 8),
    Producto("Higiene personal", "Shampoo", "1L", 50, 10),
    Producto("Higiene personal", "Desodorante", "50g", 40, 12),
    Producto("Higiene personal", "Acondicionador", "620ml", 63.50, 8),
    Producto("Higiene personal", "Gel", "450ml", 38, 10),
    Producto("Higiene personal", "Pasta dental", "135ml", 47, 12),
    Producto("Carnes", "Jamon", "290g", 37, 10),
    Producto("Carnes", "Tocino", "250g", 67, 8),
    Producto("Carnes", "Huevo", "30pz", 79, 6),
    Producto("Carnes", "Carne de res", "1Kg", 188, 8),
    Producto("Carnes", "Chorizo", "500g", 46, 8),
    Producto("Carnes", "Chuleta de cerdo", "500g", 123, 8)};

double cashback; // Se declara la variable afuera para que pueda ser utilizada globalmente

void MostrarProductos()
{
    titulo("Productos disponibles");
    int na = 1;
    vector<string> categorias;
    for (const auto &producto : productos)
    {
        if (find(categorias.begin(), categorias.end(), producto.categoria) == categorias.end())
        {
            categorias.push_back(producto.categoria);
        }
    }
    for (const auto &categoria : categorias)
    {
        cout << categoria << ":\n";
        for (const auto &producto : productos)
        {
            if (producto.categoria == categoria)
            {
                cout << na << ".- " << producto.nombre << " - " << producto.descripcion << " ... $" << producto.precio << " (Cantidad disponible: " << producto.inventario << ")\n";
                na = na + 1;
            }
        }
        cout << "\n";
    }
    cout << string(30, '*') << "\n\n";
}

void AgregarProducto(int seleccion)
{
    if (seleccion < 1 || seleccion > productos.size())
    {
        cout << "\nOpcion invalida, por favor seleccione una opcion valida: \n\n";
        return;
    }
    Producto productoSeleccionado = productos[seleccion - 1];
    int cantidad;
    if (productoSeleccionado.inventario == 0)
    {
        cout << "\nNo hay mas inventario del producto selecionado.\nPor favor seleccione algun otro producto\n";
        system("pause");
        system("cls");
        MostrarProductos();
        return;
    }
    else
    {
        cout << "\nIngrese la cantidad de productos a agregar o 0 para cancelar: ";
        cin >> cantidad;
        if (cantidad == 0)
        {
            system("cls");
            MostrarProductos();
            return;
        }
        while (cantidad < 0 || cantidad > productoSeleccionado.inventario)
        {
            if (cantidad < 0)
            {
                cout << "Cantidad invalida, por favor ingrese una cantidad valida o 0 para cancelar: " << endl;
            }
            if (cantidad > productoSeleccionado.inventario)
            {
                cout << "\nSolo hay " << productoSeleccionado.inventario << " unidades disponibles, por favor ingrese una cantidad menor o 0 para cancelar: ";
            }
            if (cantidad == 0)
            {
                return;
            }
            cin >> cantidad;
        }
        if (cantidad > 0)
        {
            for (int i = 0; i < cantidad; i++)
            {
                carrito.push_back(productoSeleccionado);
            }

            for (auto it = productos.begin(); it != productos.end(); ++it)
            {
                if (it->nombre == productoSeleccionado.nombre)
                {
                    it->inventario -= cantidad;
                    break;
                }
            }

            system("cls");
            MostrarProductos();
            if (cantidad == 1)
            {
                cout << "Se agrego un articulo '" << productoSeleccionado.nombre << "' al carrito.\n\n";
            }
            else
            {
                cout << "El producto '" << productoSeleccionado.nombre << "' se agrego al carrito " << cantidad << " veces.\n\n";
            }
        }
    }
}

void MostrarCarrito()
{
    vector<string> productosEnCarrito;
    vector<int> cantidadesEnCarrito, totalesPorProducto;
    int i, j;
    for (i = 0; i < carrito.size(); i++)
    {
        bool productoExistente = false;
        for (j = 0; j < productosEnCarrito.size(); j++)
        {
            if (productosEnCarrito[j] == carrito[i].nombre)
            {
                cantidadesEnCarrito[j]++;
                productoExistente = true;
                break;
            }
        }
        if (!productoExistente)
        {
            productosEnCarrito.push_back(carrito[i].nombre);
            cantidadesEnCarrito.push_back(1);
            totalesPorProducto.push_back(carrito[i].precio);
        }
        else
        {
            totalesPorProducto[j] += carrito[i].precio;
        }
    }

    for (int i = 0; i < productosEnCarrito.size(); i++)
    {
        cout << i + 1 << ".- '" << productosEnCarrito[i] << "' ($" << carrito[i].precio << ") _ Cantidad: " << cantidadesEnCarrito[i] << " ... Subotal: $" << totalesPorProducto[i] << "\n";
    }
    cout << "\n";
}

double generarcashback(double totalPagarDesc)
{
    cashback = (totalPagarDesc * 0.05);
    return cashback;
}

bool cantYtotal() {
    int cantidadProductos = carrito.size();
    int est;
    double totalPagar = 0.0;
    double desc = 1.0;
    string descs = "0%";
    system("cls");
    if (carrito.empty()) {
        cout << "Carrito de compras vacío\n\nRegresando al menú principal....\n";
        return true;
    } else {
        for (const auto &producto : carrito) {
            totalPagar += producto.precio;
        }
        if (cantidadProductos == 1) {
            cout << "Carrito de compras: (1 producto)" << endl;
        } else {
            cout << "Carrito de compras: (" << cantidadProductos << " productos)" << endl;
        }
        MostrarCarrito();
        cout << "¿Es estudiante?\n0 = No estudiante\n1 = Estudiante general\n2 = Estudiante BUAP\n\nSeleccione la opción: ";
        cin >> est;
        switch (est) {
            case 0: {
                desc = 1;
                descs = "0%";
                break;
            }
            case 1: {
                desc = 0.95;
                descs = "5%";
                break;
            }
            case 2: {
                desc = 0.85;
                descs = "15%";
                break;
            }
            default: {
                cout << "Opción inválida, por favor ingrese una cantidad válida: ";
                cin >> est;
                break;
            }
        }
        double totalPagarDesc = (totalPagar * desc);
        generarcashback(totalPagarDesc);
        system("cls");
        cout << "Carrito de compras: (" << cantidadProductos << " productos)" << endl;
        MostrarCarrito();
        cout << "\nTotal: $" << totalPagar << "\nDescuento: " << descs << "\nTotal a pagar: $" << totalPagarDesc << "\n\nPuntos acumulados: " << cashback << endl;
        return false;
    }
}


void MostrarProductosInventario()
{
    int inventarioTotal = 0;
    titulo("Inventario actual: ");
    vector<string> categorias;
    for (const auto &producto : productos)
    {
        if (find(categorias.begin(), categorias.end(), producto.categoria) == categorias.end())
        {
            categorias.push_back(producto.categoria);
        }
    }
    int na = 1;
    for (const auto &categoria : categorias)
    {
        int inventarioCategoria = 0;
        cout << categoria << ":\n";
        for (const auto &producto : productos)
        {
            if (producto.categoria == categoria)
            {
                cout << na << ".- " << producto.nombre << "  (Inventario: " << producto.inventario << ")\n";
                inventarioCategoria += producto.inventario;
                na = na + 1;
            }
        }
        cout << "\n";
        cout << "Inventario total de la categoria: " << categoria << " (" << inventarioCategoria << ") \n";
        cout << "\n";
        inventarioTotal += inventarioCategoria;
    }
    cout << "Inventario total de todos los productos: " << inventarioTotal << "\n******************\n\n";
}

void ModInv(int seleccion, int nuevaCantidad)
{
    if (seleccion >= 0 && seleccion < productos.size())
    {
        productos[seleccion - 1].inventario = nuevaCantidad;
    }
}
void registrarUsuario()
{
    system("cls");
    string nombreUsuario;
    int nip;
    cout << "Ingrese un nombre de usuario: ";
    cin >> nombreUsuario;
    cout << "\nIngrese un nip (5 digitos): ";
    cin >> nip;
    while (cin.fail() || to_string(nip).length() != 5)
    {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "\nNip invalido, por favor ingrese un nip de 5 digitos: ";
        cin >> nip;
    }
    FILE *users;
    users = fopen("users.txt", "r");
    char nombre[50];
    int temp1;
    double temp2;
    bool encontrado = false;
    while (fscanf(users, "%s %d %lf", nombre, &temp1, &temp2) != EOF)
    {
        if (nombreUsuario == nombre)
        {
            encontrado = true;
            cout << "\nEl usuario ya esta registrado" << endl;
            break;
        }
    }
    if (!encontrado)
    {
        users = fopen("users.txt", "a");
        fprintf(users, "%s %d %.2f\n", nombreUsuario.c_str(), nip, cashback);
        fclose(users);
        cout << "\n\nUsuario registrado exitosamente!\n\n";
    }
    fclose(users);
}

void verUsuarios()
{
    system("cls");
    FILE *users;
    users = fopen("users.txt", "r");
    char nombre[50];
    int nip;
    double cashback;
    cout << "Usuarios registrados: \n\n";
    while (fscanf(users, "%s %d %lf", nombre, &nip, &cashback) != EOF)
    {
        cout << "Nombre de usuario: " << nombre << endl;
        cout << "Cashback: $" << cashback << "\n\n";
    }
    fclose(users);
}

void guardarCashback()
{
    string nombreUsuario;
    int nip;
    bool encontrado = false;
    while (!encontrado)
    {
        char nombre[50];
        int nipusers;
        double cashbackusuario;
        ofstream tempFile;
        cout << "Ingrese su nombre de usuario: ";
        cin >> nombreUsuario;
        cin >> nip;
        FILE *users;
        users = fopen("users.txt", "r+");
        tempFile.open("temp.txt");
        while (fscanf(users, "%s %d %lf", nombre, &nipusers, &cashbackusuario) != EOF)
        {
            if (nombreUsuario == nombre && nip == nipusers)
            {
                encontrado = true;
                cashbackusuario += cashback;
                tempFile << nombre << " " << nipusers << " " << cashbackusuario << endl;
                cout << "Puntos guardados exitosamente!" << endl;
            }
        }
        fclose(users);
        tempFile.close();
        remove("users.txt");
        rename("temp.txt", "users.txt");
        if (!encontrado)
        {
            cout << "Usuario no encontrado" << endl;
            char opcion;
            do
            {
                cout << "Desea volver a intentar? (s/n): ";
                cin >> opcion;
            } while (opcion != 's' && opcion != 'n');
            if (opcion == 'n')
            {
                break;
            }
        }
    };
}

void menuCliente() {
    system("cls");
    titulo("---TIENDA EN LINEA---");
    system("pause");
    system("cls");
    MostrarProductos();
    int seleccion, client;
    bool compraFinalizada = false; // Bandera para controlar si la compra está finalizada
    while (!compraFinalizada) {
        cout << "Ingrese el numero del producto para agregar al carrito o 0 para finalizar la compra: ";
        cin >> seleccion;

        if (cin.fail()) {
            cin.clear();  // Limpiar el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignorar la entrada incorrecta
            cout << "Entrada no válida. Por favor ingrese un número." << endl;
        } else {
            if (seleccion == 0) {
                compraFinalizada = true; // Establecer la bandera para finalizar la compra
            } else {
                AgregarProducto(seleccion);
            }
        }
    }

    if (cantYtotal()) {
        return; // Salir de la función si la compra está vacía
    }

    carrito.clear();
    cout << "\nCuenta con un usuario para guardar puntos?\n1. Si\n2. No\n\nSeleccione una opcion: ";
    cin >> client;
    switch (client) {
        case 1:
            guardarCashback();
            break;
        case 2:
            cout << "\nDesea crear un usuario nuevo?\n1. Si\n2. No\n\nSeleccione una opcion: ";
            int regis;
            cin >> regis;
            switch (regis) {
                case 1:
                    registrarUsuario();
                    break;
                case 2:
                    cashback = 0;
                    break;
                default:
                    cout << "Opcion invalida, selecciona una opcion correcta: ";
                    cin >> regis;
                    break;
            }
            break;
        default:
            cout << "\nOpcion invalida, selecciona una opcion correcta: ";
            cin >> client;
            break;
    }
    system("pause");
}


void menuEmpleado() {
    bool salirMenu = false;
    while (!salirMenu) {
        system("cls");
        titulo("Bienvenido empleados");
        cout << "1.- Ver inventario de los productos\n2.- Restablecer el inventario\n3.- Ver clientes\n0.- Volver\n\nQue desea hacer?: ";
        int ele2;
        cin >> ele2;
        if (ele2 == 0) {
            salirMenu = true;
            break;
        }
        switch (ele2) {
            case 1:
                system("cls");
                MostrarProductosInventario();
                system("pause");
                break;
            case 2:
                system("cls");
                int seleccion;
                int nuevaCantidad;
                while (seleccion != 0) {
                    MostrarProductosInventario();
                    cout << "Seleccione un producto a modificar o 0 para cancelar: ";
                    cin >> seleccion;
                    if (seleccion == 0) {
                        break;
                    }
                    while (seleccion < 1 || seleccion > 36) {
                        cout << "\nOpcion invalida, por favor seleccione una opcion valida o 0 para salir: ";
                        cin >> seleccion;
                        if (seleccion == 0) {
                            break;
                        }
                    }
                    if (seleccion != 0) {
                        cout << "\nIngrese la nueva cantidad de inventario para ese producto: ";
                        cin >> nuevaCantidad;
                        system("cls");
                        ModInv(seleccion, nuevaCantidad);
                    }
                };
                break;
            case 3:
                verUsuarios();
                system("pause");
                break;
            default:
                break;
        }
    }
}

int main() {
    int ele;
    while (true) {
        system("cls");
        titulo("Bienvenido a lobo tienda");
        cout << "Como desea ingresar:\n\n1.- Cliente\n2.- Empleado\n0.- Salir\n\nSeleccione una opcion: ";
        cin >> ele;
        if (ele == 0) {
            break;
        }
        switch (ele) {
            case 1:
                menuCliente();
                break;
            case 2:
                menuEmpleado();
                break;
            default:
                cout << "\nOpcion invalida, seleccione una opcion valida.\n\n";
                system("pause");
                break;
        }
    }
    system("cls");
    titulo("Vuelva pronto");
    system("pause");
    return 0;
}
