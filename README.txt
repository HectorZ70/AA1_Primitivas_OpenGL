Este proyecto incluye el AA1/AA2/AA3

Si la tarea a corregir es la AR1 pulsa directamente la tecla F2 para ver la escena de las figuras geométricas.

-------------------------------------------------------------------------------------------------------------------

Para sacar el color naranja exacto se ha usado IA (Sacado con el siguiente promp: Teniendo en cuenta que 1.0 1.0 1.0 es blanco, como puedo sacar color naranja).

La idea de los object types ha sido dada por la IA (Sacado con el siguiente promp: Tengo un codigo de openGL, necesito que este codigo pueda mostrar varios objetos con comportamientos del shader distintos entre si, como puedo hacerlo?).

Para poder hacer las caras del cubo se ha preguntado a la IA para poder numerarlas bien, cambiando el valor de cada vertice para mayor orden (Sacado con el siguiente promp: Hazme un cubo en ascii con cada vertice tenga un numero)

También para sacar la función getKey se ha usado IA (Sacado con el siguiente promp: Que función hace que obtenga los inputs del usuario?)

Durante el proceso de creación del componente de la cámara surgió el error "símbolo externo "public: __cdecl Camera::Camera(void)" (??0Camera@@QEAA@XZ) sin resolver al que se hace referencia en la función main" al cuál se pidió la información sobre este a la IA, a lo que respondió: 
El .cpp existe pero no está incluido en el proyecto

Por ejemplo:

Camera.cpp está en la carpeta
pero Visual Studio no lo añadió al proyecto

Entonces el compilador ve la declaración, pero el linker no encuentra la implementación.

Solución en Visual Studio
Click derecho al proyecto
Agregar -> Elemento existente
selecciona Camera.cpp