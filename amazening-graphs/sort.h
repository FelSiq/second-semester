#ifndef __BSORT_H_
#define __BSORT_H_

#define BASE 10
#define MAX(A,B) (A > B) ? A : B 

/*	criteria:
	• Os menores caminhos segundo a distância Geodésica dos pontos, isto é, pela soma dos
	comprimentos dos segmentos desde a câmara inicial até uma câmara saı́da.
	• Se houver empate no caso anterior, ordenar pelo número de câmaras desde a câmara inicial
	até uma câmara saı́da.
	• Se ainda houver empate no caso anterior, ordenar levando em conta a ordem crescente dos
	ı́ndices das câmaras do caminho solução.
*/

enum{
	BUCKET_GEODESIC,
	BUCKET_PATHSIZE,
	BUCKET_PATHORDER
};

#endif