20160411
========

De forma SECRETA (no lo saben los Mojon Twins), voy a reprogramar Sir Ababol en el motor de Wo Xiang Niao Niao a ver qué tal queda, sobre todo para hacer sitio y meter secretos, que me apetece. A ver si cuela.

Por lo pronto tengo que hacer funcionar esto:

- Corregir algo del motor de Sir Ababol que hace que se cuelgue al ir a la izquierda (WTF). [lo que yo pensaba: PFACING_LEFT tenía un valor incorrecto]  [F]
- Cambiar la detección del tipo de los enemigos, que en S.A. son sólo lineales y no meten tanta mierda en "t". [F]
- Ordenar correctamente los enemigos (en la definición de metasprites). [X]
- Implementar las plataformas móviles. [X]
- Implementar el cambio de tira. Este no va por fases, va por pisos. [X]
- Cambiar la secuencia de muerte de Sir Ababol. [X]
- Implementar "hundirse" en el agua y morir. [X]
- Convertir los sonidos y las músicas a FamiTracker 2 e integrar. [X]
- Meter las pantallas: créditos, título, game over, final, y secuencia final completa. [X]
- Pausa [X]
- Objetos. Imprimir, coger, liberar cerrojos. AY... [X]

Parecía que iba a ser una tontería pero es una flipada que me va a pillar varios días. La cosa es que es algo que puedo hacer en ratos libres mientras la campaña de Verkami sigue su curso, lo que me da un mes. Y si no sale, siempre se puede tirar de la versión Enh del original que ya está hecha y funciona que es un primor.

~~

Tengo que implementar la muerte de los enemigos, pero me estoy dando cuenta de que la versión del motor es vieja y mierder y creo que tengo que dejarlo todo al máximo de perfección porque soy así de imbécil. Así que voy a darle a win merge.

Voy a codificarlo todo en en_t. Necesito:

- # del enemigo, de 1 a 4.
- 'Estoy vivo', o 'estoy muerto'.
- Contador de 16 frames.

Veamos:

	CCCCFNNN.

	En NNN -> el tipo de enemigo. 
	En F -> "1" es "muerto"
	En CCCC -> El contador. Cuando CCCCF es 11111 -> el enemigo no existe más.

Todo esto se inicializa al crear la tira, poniéndolo a cero (no habría que tocar absolutamente nada).

~~

Las plataformas móviles parece que funcionan, aunque aún tendría que probar las horizontales. De todos modos, paso a lo siguiente, el cambio de tira.

Salir por arriba si level > 0 y salir por abajo deben salir del bucle principal completamente. Salir por arriba si py < 0; salir por abajo si py > 3072. Debe conservarse px. pvy debe ser -PLAYER_VY_JUMP_MAX al subir de pantalla, y 0 al bajar.

Como ahora se inicializa el player al entrar en la tira, tendremos que cambiar eso o parametrizarlo. Voy a pensar.

~~

TOFIX: Colisión con los tiles tipo 4, hace el típico efecto escalón mierder.

Ahora toca la muerte. Tengo que implementar el safe spot: level, prx, pry. Como se cambia de nivel sí o sí, esto es más sencillo de lo que parecía.

~~

Hecho. Ahora toca... comer. Pero luego tendré que encargarme de hundir a sir ababol.

~~

Hundido. Ahora voy a meter músicas. Estoy dejando lo más chungo para el final, pero es que no tengo ni cuarenta minutos hoy y no quiero dejar nada a medias.

~~

TOFIX: Plataformas horizontales. . [F]

REMEMBER: Ver formas de procesar los 6 enemigos en un orden diferente en cada frame para introducir parpadeos en vez de sprites que desaparecen.

20160412
========

Ideas para implementar los objetos (basado un poco en el planteamiento original, aunque seguramente la implementación concreta final será diferente): [no recordaba que] en cada pantalla sólo puede haber un cerrojo y un objeto. Hay un array con los cerrojos y otro con los objetos [que tendré que generar en el conversor de la forma más adecuada y] que se emplea para detectar y para renderizar.

El conversor de mapas saca un "0" si hay un objeto o un cerrojo, aunque quizás podría ahorrar algo si codificase de otra manera, le daré una vuelta [NOTA: 45=pollo, 46=ababol, 47=llave; 15=cerrojo]. El renderer lo detecta y, al hacerlo, mira qué debe hacer: si pintar o no el cerrojo/objeto.

El código general los trataba por separado ya que al encontrar un cero no sabía bien qué hacer. Puede parecer que hacer una detección más exhaustiva podría funcionar, pero estaríamos metiendo dos IFs por tira en lugar de uno que puede desembocar en otros dos. No sé bien que hacer. En realidad me queda bastante tiempo de frame, así que lo suyo es tomar la vía que ocupe menos código.

Tengo que darle una vuelta. Voy a echar un ojal al código a ver cómo sería la modificación y decido.

Um - va a ser complicado meter en el código unrolled que tengo... ¿Podré meterlo a posteriorir? POSTERIORIRRRR. ¿Y si creo una estructura en RAM con información sobre las 128 columnas? ¿Tengo sitio para 128 bytes?

También puedo detectar la posición "X" precalculada del objeto y del cerrojo. Esto ocuparía menos memoria... Pero sería más complejo. Hum. Voy a tirar por la vía del array de 128 posiciones. Voy a irlo creando, a ver si compila.

~~

Necesito un nuevo enfoque. Esto no me da la granularidad suficiente: necesito a nivel de columna de tiles, pero me sale a nivel de chunk (2 columnas). Tengo que darle una pensada a esto.

Debe haber una forma perfe sin tener que hacer un pifostier.

Pensawr.

~~

Apunto cosas en plan brainstorming. Todos los bytes del mapa son >= 0x10 - esto no me lleva a ningún sitio.
Hm - me queda una media de 1/3 de tiempo de frame, joder, creo que puedo darle fran a eso sin ralentizar. Tengo que detectar un 0 en la tira y sustituir, o algo.

~~

A ver, empezamos de nuevo. 

- Tengo 48 pantallas. 
- Tengo 2 items por pantalla (un objeto y un cerrojo). 
- Puedo representar la posición en un byte.
- Hay un objeto por chunk que puede aparecer en la izquierda o en la derecha.
- Tengo RAM de sobra para 384 bytes y representar todos los posibles objetos.
- ¿Mejor sería simplemente guardar 3 bytes por pantalla (48x3 = 144) y detectar?

La mejor forma sería detectar y sustituir. Pero me estoy empezando a marear. Creo que hoy ha sido mal día para elegir ponerme a resolver este problema.

tengo:

- col_idx es el índice del chunk (0-127)
- (col_ctr >> 1) es el offset de la columna en el chunk (0-1)
- (col_idx >> 3) es el número de pantalla donde está.

Puedo tener obj_x, obj_y y obj_t, tres arrays de 48 elementos.

	if ((col_idx << 1) + (col_ctr >> 1) = obj_x [(col_idx >> 3)]) {
		// en esta columna que vamos a dibujar hay un objeto.

	}

Voy a duplicar el dibujado y a poner esa detección, por ahora. Sin más. A ver el rendimiento cómo queda. . . Por ahora se queda prácticamente igual. Veamos adonde nos lleva esto... Seguiré por esta vía. Pero ahora me tengo que iwr.

~~

¡Creo que he dado con ello! Usaré esa detección a posteriori para sobreescribir el buffer, retrocediendo 20 - y*2 espacios en el buffer y escribiendo ceros si la variable correspondiente está a 1. Genius!

La putada sigue siendo el tema de que tengo dos tipos de mierdas que no sé muy bien como apañar juntas. Probablemente, duplicando.

Si hago (col_idx >> 3) << 1 y tengo intercaladas ambas mierdas... Hmmm. Podía funcionar. Vamos a ver si puedo medio apañarlo.

// espera que me lío.

pantalla = (col_idx >> 3).
pantalla*2 = indice de los arrays de objetos = (col_idx >> 3) << 1.
columna real dentro de la pantalla = ((col_idx & 7) << 1) + (col_ctr >> 1)

Ok.

~~

¡¡Lo he resuelto!!

Ahora tengo que hacer estas cosas:

	[X] Detectar colisión con el objeto.
	[X] Detectar colisión con el hotspot.

20160413
========

Entre otras cosas envío los sprites de los enemigos a la copia de la OAM en un orden diferente cada frame, evitando desapariciones (ahora hay parpadeos: BIEN).

Ahora voy a hacer el HUD. Para ello crearé un metasprite con los elementos fijos para ahorrar código.

	0x51 = vidas
	0x52 = ababol
	0x53 = llave
	0x54 = 'x'
	0x55... = '0'

Todo esto lo quiero encasquetar al final de la OAM.

La parte fija del HUD son 6 elementos, o sea, 24 bytes. La parte variable son 4 elementos, o sea, 16 bytes. Total: 40 bytes. Lo creo al principio, en la parte fija de mi mierda, justo después del player, en 28. El inicio del parque movil se mueve, por tanto, a 68.

~~

No sé en qué punto pasó esto, pero hay algo jodiéndomelo todo. Los sonidos no suenan, la música se corrompe, aparecen sprites donde no son... Uf.

--

Medio resuelto: estaba usando una versión de famitracker que no se llevaba bien con el conversor que estaba usando (eran versiones diferentes). Aún sigue sonando raro, es un misterio. A unas malas, al final del proyecto, cojo y le encasquieto famitracker 1 y los archivos antiguos, que sí sonaban bien.

Ahora he visto que los hotspots no funcionan en otro piso que no sea el segundo. Probablemente no se esté actualizando algo.

Medio resuelto: ahora va, a medias. Los detecta y reacciona, pero no es capaz de "tapar" los objetos. Algo falla en el scroller. 

Fixed: No era en el scroller, era que a la hora de poner a 0 la posición correcta del array, no se estaba añadiendo el offset de la tira.

Ya va todo guay (menos el puto sonido del salto).

Ahora lo que quedaría sería:

	[X] Tocar ppu_mask, desactivar la tira izquierda, activar +B en el subsuelo.
	[X] Intentar introducir las subsecciones, pensar en una forma de organizar la info (*)
	[X] Condición de fin de juego.
	[ ] Pantalla (C)
	[ ] Título
	[ ] Game Over
	[ ] Ending sequence

Con esto hecho, ya puedo empezar a meter el modo secreto de juego = la DIABLURA.

(*) En el cambio de strip se consulta la posición X de tile del centro del sprite del prota con una serie de datos almacenados en un array.

Array de cambios:

	0  1  2  3
	a, b, c, d

Si por ejemplo a <= x <= b, se toma que es la sección del índice de "a", o sea, la 0.

array de limites

	l_x1 [] y l_x2 [], 

indizados por 'a', establecen los limites de la camara para esa sección.

Esto va a funcionar de la muerter! Mañana le meto.

20160414
========

Es tontería usar dos arrays, con uno me vale y además todo es mucho más flexible. Tengo que definir, en pixels, las diferentes areas. Tengo que, además, definir tres arrays, uno por nivel, y enlazarlos todos en uno global sobre el que iterar. 

Voy a apuntar aquí las áreas necesarias.

	Level 0:
		- Tiles 0-255 -> 0x16 - 255x16+15 = 0 .. 4095

	Level 1:
		- Tiles 0-29 -> 0x16 - 29x16+15 = 0 .. 479
		- Tiles 30-62 -> 480 .. 1007
		- Tiles 63-255 ->  1008 .. 4096

	Level 2:
		- Tiles 0-255 -> 0x16 - 255x16+15 = 0 .. 4095

Necesito un "end marker", puede ser cualquier valor que no tenga sentido (de forma "conceptual", para el motor todos los valores tienen sentido), como por ejemplo, el 1.

También puedo poner como end marker el puto 4096...

Los arrays serían, pues...

	const unsigned int sections_level_0 [] = {0, 4096};
	const unsigned int sections_level_1 [] = {0, 480, 1008, 4096};
	//const unsigned int sections_level_2 [] = {0, 4096}; // me lo ahorro, es igual que el 0.
	const unsigned int *sections [] = { 
		sections_level_0, sections_level_1, /*sections_level_2*/ sections_level_0
	};

Con esto y algo de lógica lo tengou

¡¡Genial!! Como un tiro. Pero he tenido que poner todo como signed int que si no cc65 se hacía la picha un lío.

~~ 

Estoy pensando en montar la mierda extra antes de las pantallas, para así tener lo "importante" (wink) resuelto y luego pelearme con apretar todo lo que falte.

~~

Ya he metido los nuevos sprites y paletas y he probado a activarlos, y todo va guay. Mañana pondré el cambio de reglas:

- Salto sobre enemigo desactivado.
- Botón "B" saca el paraguas, rollo hitter de MK2.
- Colisión hitter->enemigo.

TODO: Ver cómo activar la compensación NTSC->PAL, que parece desactivada por alguna razón...

~~

Ya funciona todo! (lo de la compensación, misteriosamente, no ¿será por el emu?). Ya está el motor alternativo de la diablura metido. Ahora tengo que terminar el resto de las cosas.

Lo apunto, para la lista de todo/tofix/torefine: En el juego alternativo, hay que buscar un único ababol azul que abrirá la puerta de la celda donde está Sir Ababol (en la pantalla final). De hecho, ¡lo suyo es que el juego alternativo empiece justo ahí! Tengo que ver si ese recorrido es posible con las llaves tal y como están.

20160415
========

Parece que el recorrido en dirección contraria es posible, pero lo suyo es que también haya que recoger todos los ababoles... ¡Y volver! Voy a pensar en la forma más sencilla de integrar todo esto (seguro que es muy fácil), lo dejo listo, y me pongo a hacer las pantallas de título, final, game over, y tal. 

Y si queda sitio, me invento otro modo de juego más y ya lo peto :D

~~

TODO: Desacoplar colisiones con los enemigos del update, para que todo esté ya renderizado cuando ocurran.

~~

Tengo que pulir mil cosas, pero básicamente ya está todo y aún sobra sitio por todos lados. Tengo que pensar qué meter. Tengo que repasar la lista de tofix y todo. Tengo que meter una pantalla de introducción para le juego oculto.

		    4
	COLLECT BLUE FLOWERS  6
	THEN HEAD TO THE      8
	UNDERGROUND DUNGEON.  10

	PRESS B TO THROW      22
	THE UMBRELLA          24

~~

20160416
========

Bueno, está hecho. Al final he metido incluso otro juego XD

- SELECT+A+UP para jugar con Zazelle.
- A+B+DOWN para jugar con Johnny Limite.

Si pulsas DERECHA sale un cohete rosa por el cielo nintendo.

Aún queda algo de sitio, puede que meta alguna tontería o broma más.

20160419
========

Estamos en el momento de pulir esto al máximo. A ver si convenzo a David para que se curre una música mejor o retoque la que ya hay. Tengo, además, que usar una bandera en el bucle principal para que, si está levantada, no se pare la música al salir y no se relance al entrar, para que la música no se reinicie en los cambios de tira.

20160420
========

Nice try but no banana, la música la apaño yo. He visto que lo que jode los samples es "algo" que hay en el canal 1. Hay algo en este canal que parece darle prioridad sobre la música. No recuerdo bien, pero creo recordar haber leído en algún sitio que la prioridad funcionaba por volumenes. ¿Instrumentos demasiado altos? Voy a mirar el salto y veo los volúmenes, por ejemplo.

20160422
========

Una buena forma de probar a repararlo puede ser intercambiar canales 1 y 2 entre sí, lo cual dejaré para más adelante.

Voy a revisar las colisiones, que no me queda claro que las corrigiese.

~

Las colisiones están bien. Lo que detesto del juego es el "clic" tras morir, que los enemigos avanzan un frame. Voy a intentar analizar por qué ocurre y ver cómo solucionarlo.

Vale -> es una tontería. Al separar la colisión, había alterado el orden de las cosas. Lo suyo es que, en el bucle principal, el orden sea "mover" "actualizar" "detectar muerte" "detectar fin de bucle". Así creo que debería eliminarse el click... veamos.

Hecho, y funcionando como un puto tiro.

~ 

¿Podré cambiar el orden de los canales usando sustituciones y expresiones regulares sobre el export en TXT y luego reimportarlo? UM. Voy a intentarlo. Debería poder.

	ROW 00 : B-3 0A . ... : B-2 20 . ... : B-2 1C . ... : 0-# 04 . ... : ... .. . ...

Eso de ahí es una fila de un patrón. Tengo que intercambiar las dos primeras columnas. Veamos...

	ROW [A-Z0-9]{2} : [A-Z0-9\-\.#]{3} [A-Z0-9\.]{2} \. [.]{3} : [A-Z0-9\-\.#]{3} [A-Z0-9\.]{2} \. [.]{3}

Eso de ahí arriba selecciona las dos primeras columnas de cada fila... Ahora sólo tengo que usar sustituciones. Veamos:

	(ROW [A-Z0-9]{2})( : [A-Z0-9\-\.#]{3} [A-Z0-9\.]{2} \. [.]{3})( : [A-Z0-9\-\.#]{3} [A-Z0-9\.]{2} \. [.]{3})

Así las selecciones para la primera y segunda columnas son iguales. Sólo tengo que usar sustitución y poner 1-3-2. Probamos y... Funciona.

¿Y como suena? -> BIEN !! No se jode tanto la música con los saltos. Pero la conversión automática no ha sido del todo exitosa, porque ahora tengo que arreglar también la lista de reproducción, intercambiando ambos canales. Eso es fácil. Como lo logre automáticamente, va a ser un JRAN triunfo y, por fin, el juego estará mejorado en todos los aspectos :-)

REMEMBER: Por ahora lo estoy haciendo todo sobre el export en .txt pero lo suyo sería reimportar y guardar un .ftm para el futuro.
HECHO.

Y días más tarde: Juego probao y terminao y todo. Fue muy divertido y bonito ver como Ángel descubría los añadidos sin esperárselos.
