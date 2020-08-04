/*
Problema del proveedor y el consumidor

H1 (agregar contenido) -> RECURSO <- H2 (sacar cntenido)

Si el RECURSO esta vacio -> H1 podria agregar contenido
Si el RECURSO esta vacio -> H2 podría sacar contenido???

Si el RECURSO esta lleno -> H1 podria agregar contenido???
Si el RECURSO esta lleno -> H2 podria sacar contenido

RECURSO -> SECCION CRITICA del CODIGO

EXCLUSION MUTUA -> LOCK -> CERROJO
EXCLUSION CONTROLADA -> SEMAPHORE -> SEMAFORO
*/