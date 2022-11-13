import { createRequire } from "module";
const require = createRequire(import.meta.url);

const Machine = require("../build/Release/machine");
const Game = require("../build/Release/game");

const populationSize = 100;
const MEMORY_LENGTH = 64;
const maxGeneration = 1000;

const generateArrayBufferMemory = size => {
	const memory = new ArrayBuffer(size);
	const memoryView = new Uint8Array(memory);
	for (let i = 0; i < size; i++) {
		memoryView[ i ] = Math.floor(Math.random() * 255);
	}

	return memory;
};

function genRawPopulation(size) {
	const population = [];
	for (let i = 0; i < size; i++) {
		population.push(generateArrayBufferMemory(MEMORY_LENGTH));
	}

	return population;
}

function evaluatePopulation(rawPopulation) {
	const population = [];
	for (const gen of rawPopulation) {
		const moves = Machine.run(gen, MEMORY_LENGTH);
		const stores = Game.run(moves);
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
	const childView = new Uint8Array(child);

	const crossoverPoint = Math.floor(Math.random() * MEMORY_LENGTH);
	for (let i = 0; i < crossoverPoint; i++) {
		childView[ i ] = parent1View[ i ];
	}

	for (let i = crossoverPoint; i < MEMORY_LENGTH; i++) {
		childView[ i ] = parent2View[ i ];
	}

	return child;
}

function generateNextPopulation(population) {
	// Sort population by stores
	const sortedPopulation = population.sort((a, b) => b.stores - a.stores);

	// Get the top 50% of the population
	const topPopulation = sortedPopulation.slice(0, Math.floor(populationSize / 2));
	// Create other 50% of the population by crossover and mutation
	let bottomPopulation = [];
	for (let i = 0; i < Math.floor(populationSize / 2); i++) {
		const parent1 = topPopulation[ i ];
		const parent2 = topPopulation[ Math.floor(Math.random() * Math.floor(populationSize / 2)) ];
		const child = crossoverGens(parent1.gen, parent2.gen);
		bottomPopulation.push(child);
	}

	// Mutate the bottom population
	for (let i = 0; i < bottomPopulation.length; i++) {
		bottomPopulation[ i ] = mutateGen(bottomPopulation[ i ]);
	}

	// Evaluate the new population
	bottomPopulation = evaluatePopulation(bottomPopulation);
	// Merge the top and bottom population
	const nextPopulation = topPopulation.concat(bottomPopulation);


	return nextPopulation;
}


function run() {
	console.log("Generating population...");

	const rawPopulation = genRawPopulation(populationSize);
	let population = evaluatePopulation(rawPopulation);
	for (let generation = 0; generation < maxGeneration; generation++) {
		population = generateNextPopulation(population);
		if (population[ 0 ].stores === 5) {
			console.log("Found solution!");
			break;
		} else {
			console.clear();
			console.log(`Generation ${generation}`);
		}
	}

	console.log(`Best gen: \n Moves: ${population[ 0 ].moves} \n Stores: ${population[ 0 ].stores}`);

}

async function main() {
	run();
}

await main();
