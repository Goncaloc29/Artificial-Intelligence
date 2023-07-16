/*
Efolio A - Introdução à Inteligência Artificial
Entrega: 17/04/2023
Aluno: Gonçalo Caraça
Numero: 2000130

Instruções para execução:

1. Instalar o Node.js
2. Abrir o terminal e navegar até à pasta onde se encontra o ficheiro
3. Executar o comando "node efolioA.js"
 */

/**
 * Contador de fronteiras
 */
function contaFronteiras(matriz) {
	let contador = 0;
	for (let i = 0; i < matriz.length; i++) {
		let m1 = matriz[i];
		for (let j = 0; j < m1.length; j++) {
			if (m1[j + 1] != undefined && m1[j] != m1[j + 1]) {
				/**
				 * Conta fronteiras horizontais
				 */
				contador++;
			}
			if (matriz[i + 1] !== undefined) {
				let m2 = matriz[i + 1];
				if (m1[j] != m2[j]) {
					/**
					 * Conta fronteiras verticais
					 */
					contador++;
				}
			}
		}
	}
	return contador;
}

/**
 * Copiar array para o novo array para não fazer referência ao antigo array
 */
function copiaArray(matriz) {
	let copy = [];
	for (let i = 0; i < matriz.length; i++) {
		copy.push(matriz[i].slice());
	}
	return copy;
}

/**
 * Algoritmo Procura Profundidade
 */
function procuraProfundidade(matriz) {
	let matrizEsquerda = copiaArray(matriz);
	let matrizCima = copiaArray(matriz);
	let matrizDireita = copiaArray(matriz);
	let matrizBaixo = copiaArray(matriz);
	for (let i = 0; i < matriz.length; i++) {
		let m1 = matriz[i];
		for (let j = 0; j < m1.length; j++) {
			let matrizAfter = [];
			//esquerda
			if (m1[j - 1] != undefined && m1[j] != m1[j - 1]) {
				let m1Copy = m1.slice();
				let m1B = m1Copy[j];
				m1Copy[j] = m1Copy[j - 1];
				m1Copy[j - 1] = m1B;
				matrizEsquerda[i] = m1Copy;
				if (!validaMatrizesGeradas(matrizEsquerda)) {
					matrizesGeradas.push(matrizEsquerda);
					if (contaFronteiras(matrizEsquerda) <= fronteiras) {
						matrizAfter = copiaArray(matrizEsquerda);
						fronteiras = contaFronteiras(matrizEsquerda);
						expansao++;
					}
				}
			}
			//cima
			if (matriz[i - 1] != undefined && m1[j] != matriz[i - 1][j]) {
				let arrayAtualCopy = m1.slice();
				let valorIndiceJAtual = arrayAtualCopy[j];
				let arrayCimaCopy = matriz[i - 1].slice();
				arrayAtualCopy[j] = arrayCimaCopy[j];
				arrayCimaCopy[j] = valorIndiceJAtual;
				matrizCima[i] = arrayAtualCopy;
				matrizCima[i - 1] = arrayCimaCopy;
				if (!validaMatrizesGeradas(matrizCima)) {
					matrizesGeradas.push(matrizCima);
					if (contaFronteiras(matrizCima) <= fronteiras) {
						matrizAfter = copiaArray(matrizCima);
						fronteiras = contaFronteiras(matrizCima);
						expansao++;
					}
				}
			}
			//direita
			if (m1[j + 1] != undefined && m1[j] != m1[j + 1]) {
				let m1Copy = m1.slice();
				let m1B = m1Copy[j];
				m1Copy[j] = m1Copy[j + 1];
				m1Copy[j + 1] = m1B;
				matrizDireita[i] = m1Copy;
				if (!validaMatrizesGeradas(matrizDireita)) {
					matrizesGeradas.push(matrizDireita);
					if (contaFronteiras(matrizDireita) <= fronteiras) {
						matrizAfter = copiaArray(matrizDireita);
						fronteiras = contaFronteiras(matrizDireita);
						expansao++;
					}
				}
			}
			//baixo
			if (matriz[i + 1] != undefined && m1[j] != matriz[i + 1][j]) {
				let arrayAtualCopy = m1.slice();
				let valorIndiceJAtual = arrayAtualCopy[j];
				let arrayCimaCopy = matriz[i + 1].slice();
				arrayAtualCopy[j] = arrayCimaCopy[j];
				arrayCimaCopy[j] = valorIndiceJAtual;
				matrizBaixo[i] = arrayAtualCopy;
				matrizBaixo[i + 1] = arrayCimaCopy;
				if (!validaMatrizesGeradas(matrizBaixo)) {
					matrizesGeradas.push(matrizBaixo);
					if (contaFronteiras(matrizBaixo) <= fronteiras) {
						matrizAfter = copiaArray(matrizBaixo);
						fronteiras = contaFronteiras(matrizBaixo);
						expansao++;
					}
				}
			}

			if (matrizAfter.length !== 0) {
				matriz = copiaArray(matrizAfter);
				m1 = matriz[i];
				if (fronteiras < menorFronteira) {
					menorFronteira = fronteiras;
					matrizMenorFronteira = copiaArray(matriz);
				}
			}
			duracao = Date.now() - antes;
			if (duracao / 1000 >= segundos) {
				break;
			}
			matrizEsquerda = copiaArray(matriz);
			matrizCima = copiaArray(matriz);
			matrizDireita = copiaArray(matriz);
			matrizBaixo = copiaArray(matriz);
		}
	}
	return matriz;
}

/**
 * Verifica se já gerou aquela matriz
 */
function validaMatrizesGeradas(matriz) {
	let existe = false;
	for (let i = 0; i < matrizesGeradas.length; i++) {
		if (matriz.toLocaleString() === matrizesGeradas[i].toLocaleString()) {
			existe = true;
		}
	}
	return existe;
}

/**
 * Ajuste do array da matriz para entrar no printResultado de cada matriz W1 e W2
 */
function printMatriz(matriz) {
	let print = "";
	for (let i = 0; i < matriz.length; i++) {
		print += matriz[i] + "\n";
	}
	return print;
}

/* Matrizes a testar */
let matrizes = [
	{
		matriz: [
			[1, 2, 3],
			[1, 2, 2],
			[3, 3, 1],
		],
		repeticoes: [
			{
				fronteiraEsperada: 6,
				printResultado: "Resultado 1 W1\n",
			},
			{
				fronteiraEsperada: 5,
				printResultado: "Resultado 1 W2\n",
			},
		],
	},
	{
		matriz: [
			[1, 2, 2, 2],
			[1, 2, 1, 1],
		],
		repeticoes: [
			{
				fronteiraEsperada: 4,
				printResultado: "Resultado 2 W1\n",
			},
			{
				fronteiraEsperada: 2,
				printResultado: "Resultado 2 W2\n",
			},
		],
	},
	{
		matriz: [
			[1, 2, 2, 2],
			[1, 3, 3, 3],
			[1, 2, 1, 1],
			[1, 1, 3, 2],
		],
		repeticoes: [
			{
				fronteiraEsperada: 10,
				printResultado: "Resultado 3 W1\n",
			},
			{
				fronteiraEsperada: 9,
				printResultado: "Resultado 3 W2\n",
			},
		],
	},
	{
		matriz: [
			[1, 1, 2, 1, 1],
			[2, 2, 1, 2, 1],
			[1, 1, 2, 1, 2],
			[2, 1, 1, 2, 1],
		],
		repeticoes: [
			{
				fronteiraEsperada: 10,
				printResultado: "Resultado 4 W1\n",
			},
			{
				fronteiraEsperada: 9,
				printResultado: "Resultado 4 W2\n",
			},
		],
	},
	{
		matriz: [
			[1, 2, 2, 2, 2, 1, 2, 2, 2, 2],
			[1, 3, 3, 3, 4, 1, 3, 3, 3, 4],
			[1, 2, 1, 4, 3, 1, 2, 1, 4, 3],
			[1, 4, 4, 4, 3, 1, 4, 4, 4, 3],
		],
		repeticoes: [
			{
				fronteiraEsperada: 30,
				printResultado: "Resultado 5 W1\n",
			},
			{
				fronteiraEsperada: 25,
				printResultado: "Resultado 5 W2\n",
			},
		],
	},
	{
		matriz: [
			[1, 1, 2, 1, 1, 1, 1, 2, 1, 1],
			[2, 2, 1, 2, 1, 2, 2, 1, 2, 1],
			[1, 1, 2, 1, 2, 1, 1, 2, 1, 2],
			[2, 1, 1, 2, 1, 2, 1, 1, 2, 1],
			[1, 1, 2, 1, 1, 1, 1, 2, 1, 1],
			[2, 2, 1, 2, 1, 2, 2, 1, 2, 1],
			[1, 1, 2, 1, 2, 1, 1, 2, 1, 2],
			[2, 1, 1, 2, 1, 2, 1, 1, 2, 1],
		],
		repeticoes: [
			{
				fronteiraEsperada: 41,
				printResultado: "Resultado 6 W1\n",
			},
			{
				fronteiraEsperada: 35,
				printResultado: "Resultado 6 W2\n",
			},
		],
	},
	{
		matriz: [
			[1, 1, 2, 8, 8, 1, 4, 3, 1, 4],
			[2, 2, 1, 8, 3, 8, 4, 3, 2, 1],
			[1, 1, 8, 8, 3, 1, 6, 2, 1, 4],
			[2, 1, 1, 3, 1, 2, 1, 1, 4, 4],
			[1, 7, 7, 3, 1, 1, 5, 6, 4, 4],
			[2, 2, 1, 3, 1, 2, 2, 1, 6, 6],
			[1, 7, 2, 7, 5, 5, 5, 5, 1, 6],
			[2, 7, 7, 7, 1, 5, 5, 1, 6, 6],
		],
		repeticoes: [
			{
				fronteiraEsperada: 70,
				printResultado: "Resultado 7 W1\n",
			},
			{
				fronteiraEsperada: 62,
				printResultado: "Resultado 7 W2\n",
			},
		],
	},
];

let matrizesGeradas = [];
let fronteiras = 0;
let expansao = 0;
let menorFronteira = 0;
let matrizMenorFronteira = [];
/**
 * Máximo de segundos para cada geração de resultado
 */
let segundos = 60;

for (let k = 0; k < matrizes.length; k++) {
	let testeMatriz = matrizes[k];
	for (let i = 0; i < testeMatriz.repeticoes.length; i++) {
		matrizesGeradas = [];
		let w = testeMatriz.repeticoes[i];
		let matriz = testeMatriz.matriz;

		let fronteiraEsperada = w.fronteiraEsperada;
		fronteiras = contaFronteiras(matriz);
		expansao = 0;
		menorFronteira = contaFronteiras(matriz);
		matrizMenorFronteira = copiaArray(matriz);
		let printResultado = "";

		printResultado += "Fronteiras: " + fronteiras + "\n";

		var antes = Date.now();

		var duracao = Date.now() - antes;
		while (true) {
			matriz = procuraProfundidade(matriz);
			for (let j = 0; j < matrizesGeradas.length; j++) {
				if (fronteiraEsperada >= menorFronteira) {
					break;
				}
				if (duracao / 1000 >= segundos) {
					break;
				}
				matriz = procuraProfundidade(matrizesGeradas[j]);
			}
			if (fronteiraEsperada >= menorFronteira) {
				break;
			}
			if (duracao / 1000 >= segundos) {
				break;
			}
		}

		printResultado =
			"===============================\n" +
			"Expansão: " +
			expansao +
			"\n" +
			"Geração: " +
			matrizesGeradas.length +
			"\n";
		if (menorFronteira <= fronteiraEsperada) {
			printResultado += "Matriz com Menor Fronteira\n";
			printResultado += printMatriz(matrizMenorFronteira);
		} else {
			printResultado += "Não Resolvido / Tempo excedido!\n";
		}
		printResultado += "Duração: " + duracao / 1000 + " segundos\n";
		printResultado += "Fronteiras: " + menorFronteira + "\n";
		testeMatriz.repeticoes[i].printResultado += printResultado;
	}
	for (let l = 0; l < testeMatriz.repeticoes.length; l++) {
		console.log(testeMatriz.repeticoes[l].printResultado);
	}
}
