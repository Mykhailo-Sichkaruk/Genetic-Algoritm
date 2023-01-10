import fs from "fs";
import { createRequire } from "module";
const require = createRequire(import.meta.url);

const Machine = require("../build/Release/machine");
const Game = require("../build/Release/game");

const MEMORY_LENGTH = 64;

function logGame(grabbedStores) {
	fs.appendFileSync("grabbedStores.txt", grabbedStores.join("\n"));
}

function generateInstructions(size) {
	// Create a memory buffer of Unsigned 8-bit Integers
	const memory = new ArrayBuffer(size);
	const memoryView = new Uint8Array(memory);

	// Fill the memory with random instructions
	for (let i = 0; i < size; i++) {
		memoryView[ i ] = Math.floor(Math.random() * 255);
	}

	return memory;
}

function genRawPopulation(size) {
	const population = [];
	for (let i = 0; i < size; i++) {
		population.push(generateInstructions(MEMORY_LENGTH));
	}

	return population;
}

function evaluatePopulation(rawPopulation) {
	const population = [];
	for (const gen of rawPopulation) {
		const moves = Machine.run(gen, MEMORY_LENGTH);
		const stores = Game.run(moves, logGame);
		population.push({
			gen,
			stores,
			moves,
		});
	}

	return population;
}

function mutateGen(gen) {
	const genView = new Uint8Array(gen);
	const index = Math.floor(Math.random() * MEMORY_LENGTH);
	genView[ index ] = Math.floor(Math.random() * 255);

	return gen;
}

function crossoverGens(parent1, parent2) {
	const parent1View = new Uint8Array(parent1);
	const parent2View = new Uint8Array(parent2);

	const child = new ArrayBuffer(MEMORY_LENGTH);

	// Crossover first half of the parent1 with second half of the parent2
	const childView = new Uint8Array(child);
	for (let i = 0; i < MEMORY_LENGTH / 2; i++) {
		childView[ i ] = parent1View[ i ];
	}
	for (let i = MEMORY_LENGTH / 2; i < MEMORY_LENGTH; i++) {
		childView[ i ] = parent2View[ i ];
	}

	return child;
}

export { genRawPopulation, evaluatePopulation, mutateGen, crossoverGens };
