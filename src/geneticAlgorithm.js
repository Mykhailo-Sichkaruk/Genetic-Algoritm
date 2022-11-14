import { createRequire } from "module";
const require = createRequire(import.meta.url);

const Machine = require("../build/Release/machine");
const Game = require("../build/Release/game");
const plotly = require("plotly")("Mykhailo-Sichkaruk", "gTYne1CKhcfYLhiNfHNe");


const populationSize = 100;
const MEMORY_LENGTH = 64;
const maxGeneration = 10000;

const 	generateArrayBufferMemory = size => {
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
function genWitnEliteCrossover(population) {
	// Sort population by stores
	population.sort((a, b) => b.stores - a.stores);

	// Get top 10% of population
	const topPopulation = population.slice(0, Math.floor(populationSize / 10));

	// Croccover each top gen with each other
	let newPopulation = [];
	for (const parent1 of topPopulation) {
		for (const parent2 of topPopulation) {
			newPopulation.push(crossoverGens(parent1.gen, parent2.gen));
		}
	}

	// Evaluate the new population
	newPopulation = evaluatePopulation(newPopulation);

	// Sort population by stores
	newPopulation.sort((a, b) => b.stores - a.stores);
	// Get top 90% of population
	const nextPopulation = newPopulation.slice(0, Math.floor(populationSize / 10 * 9));

	// Get bottom 10% of population
	const bottomPopulation = population.slice(Math.floor(populationSize / 10 * 9), populationSize);
	// Merge top and bottom population
	const mergedPopulation = nextPopulation.concat(bottomPopulation);

	// Calculate overall stores of the top 10% of the population
	const top10Population = mergedPopulation.slice(0, Math.floor(populationSize / 10));
	const overallStores = top10Population.reduce((acc, gen) => acc + gen.stores, 0);

	return [ mergedPopulation, overallStores];
}

function genWithCrossover(population) {
	// Sort population by stores
	const sortedPopulation = population.sort((a, b) => b.stores - a.stores);

	// Get the top 50% of the population
	const topPopulation = sortedPopulation.slice(0, Math.floor(populationSize / 2));
	// Create other 50% of the population by crossover
	let bottomPopulation = [];
	for (let i = 0; i < Math.floor(populationSize / 2); i++) {
		const parent1 = topPopulation[ i ];
		const parent2 = topPopulation[ Math.floor(Math.random() * Math.floor(populationSize / 2)) ];
		const child = crossoverGens(parent1.gen, parent2.gen);
		bottomPopulation.push(child);
	}

	// Evaluate the new population
	bottomPopulation = evaluatePopulation(bottomPopulation);
	// Merge the top and bottom population
	let nextPopulation = topPopulation.concat(bottomPopulation);

	// Sort population by stores
	nextPopulation = nextPopulation.sort((a, b) => b.stores - a.stores);

	// Calculate overall stores of the top 10% of the population
	const top10Population = nextPopulation.slice(0, Math.floor(populationSize / 10));
	const overallStores = top10Population.reduce((acc, gen) => acc + gen.stores, 0);

	return [ nextPopulation, overallStores ];
}

function VadimCrossover(gen1, gen2) {
	const gen1View = new Uint8Array(gen1);
	const gen2View = new Uint8Array(gen2);

	const childs = [];
	const child = new ArrayBuffer(MEMORY_LENGTH);

	// Crossover first half of the parent1 with first half of the parent2
	const childView = new Uint8Array(child);
	for (let i = 0; i < MEMORY_LENGTH / 2; i++) {
		childView[ i ] = gen1View[ i ];
	}
	for (let i = MEMORY_LENGTH / 2; i < MEMORY_LENGTH; i++) {
		childView[ i ] = gen2View[ i ];
	}
	childs.push(child);

	// Crossover first half of the parent1 with second half of the parent2
	const child2 = new ArrayBuffer(MEMORY_LENGTH);
	const child2View = new Uint8Array(child2);
	for (let i = 0; i < MEMORY_LENGTH / 2; i++) {
		child2View[ i ] = gen1View[ i ];
	}
	for (let i = MEMORY_LENGTH / 2; i < MEMORY_LENGTH; i++) {
		child2View[ i ] = gen2View[ i ];
	}
	childs.push(child2);

	// Crossover second half of the parent1 with first half of the parent2
	const child3 = new ArrayBuffer(MEMORY_LENGTH);
	const child3View = new Uint8Array(child3);
	for (let i = 0; i < MEMORY_LENGTH / 2; i++) {
		child3View[ i ] = gen2View[ i ];
	}
	for (let i = MEMORY_LENGTH / 2; i < MEMORY_LENGTH; i++) {
		child3View[ i ] = gen1View[ i ];
	}
	childs.push(child3);

	// Crossover second half of the parent1 with second half of the parent2
	const child4 = new ArrayBuffer(MEMORY_LENGTH);
	const child4View = new Uint8Array(child4);
	for (let i = 0; i < MEMORY_LENGTH / 2; i++) {
		child4View[ i ] = gen2View[ i ];
	}
	for (let i = MEMORY_LENGTH / 2; i < MEMORY_LENGTH; i++) {
		child4View[ i ] = gen1View[ i ];
	}
	childs.push(child4);

	return childs;
}

function Vadim(population) {
	population.sort((a, b) => b.stores - a.stores);

	// Get top 25% of population
	const topPopulation = population.slice(0, Math.floor(populationSize / 4));
	// Croccover each top gen with each other
	const newPopulation = [];
	for (let i = 0; i < topPopulation.length; i++) {
		for (let j = i + 1; j < topPopulation.length; j++) {
			const childs = VadimCrossover(topPopulation[ i ].gen, topPopulation[ j ].gen);
			newPopulation.push(...childs);
		}
	}

	// Evaluate the new population
	const evaluatedPopulation = evaluatePopulation(newPopulation);

	// Sort population by stores
	evaluatedPopulation.sort((a, b) => b.stores - a.stores);

	// Get only populationSize of the population
	const nextPopulation = evaluatedPopulation.slice(0, populationSize);
	// Calculate overall stores of the top 10% of the population
	const top10Population = nextPopulation.slice(0, Math.floor(populationSize / 10));
	const overallStores = top10Population.reduce((acc, gen) => acc + gen.stores, 0);

	return [ nextPopulation, overallStores ];
}

function genWIthGeneticAlgorithm(population) {
	// Sort population by stores
	const sortedPopulation = population.sort((a, b) => b.stores - a.stores);

	// Get the top 50% of the population
	const topPopulation = sortedPopulation.slice(0, Math.floor(populationSize / 2));
	// Create other 50% of the population by crossover
	const bottomPopulation = [];
	for (let i = 0; i < Math.floor(populationSize / 2); i++) {
		const parent1 = topPopulation[ i ];
		const parent2 = topPopulation[ Math.floor(Math.random() * Math.floor(populationSize / 2)) ];
		const child = crossoverGens(parent1.gen, parent2.gen);
		bottomPopulation.push(child);
	}

	// Mutate childrens of the population
	let mutatedPopulation = [];
	for (const child of bottomPopulation) {
		const gen = mutateGen(child);
		mutatedPopulation.push(gen);
	}

	// Evaluate the new population
	mutatedPopulation = evaluatePopulation(mutatedPopulation);
	// Merge the top and bottom population
	let nextPopulation = topPopulation.concat(mutatedPopulation);

	// Sort population by stores
	nextPopulation = nextPopulation.sort((a, b) => b.stores - a.stores);

	// Calculate overall stores of the top 10% of the population
	const top10Population = nextPopulation.slice(0, Math.floor(populationSize / 10));

	const overallStores = top10Population.reduce((acc, gen) => acc + gen.stores, 0);

	return [ nextPopulation, overallStores ];
}

function genWithMutation(population) {
	// Sort population by stores
	const sortedPopulation = population.sort((a, b) => b.stores - a.stores);

	// Get the top 50% of the population
	const topPopulation = sortedPopulation.slice(0, Math.floor(populationSize / 2));
	// Create other 50% of the population by crossover and mutation
	let bottomPopulation = [];
	for (let i = 0; i < Math.floor(populationSize / 2); i++) {
		const parent = topPopulation[ i ];
		const child = mutateGen(parent.gen);
		bottomPopulation.push(child);
	}

	// Evaluate the new population
	bottomPopulation = evaluatePopulation(bottomPopulation);
	// Merge the top and bottom population
	let nextPopulation = topPopulation.concat(bottomPopulation);

	// Sort population by stores
	nextPopulation = nextPopulation.sort((a, b) => b.stores - a.stores);

	// Calculate overall stores of the top 10% of the population
	const top10Population = nextPopulation.slice(0, Math.floor(populationSize / 10));
	const overallStores = top10Population.reduce((acc, gen) => acc + gen.stores, 0);

	return [ nextPopulation, overallStores ];
}

function run(mode, rawPopulation,) {
	console.log("Generating population...");

	const modeFunction = {
		"mutation": genWithMutation,
		"crossover": genWithCrossover,
		"elite-crossover": genWitnEliteCrossover,
		"vadim": Vadim,
		"genetic-algorithm": genWIthGeneticAlgorithm
	};
	const stats = {
		x: [],
		y: [],
		mode: "markers",
		type: "scatter",
		name: mode,
	};
	let population = evaluatePopulation(rawPopulation);
	let overallStores = population.reduce((acc, gen) => acc + gen.stores, 0);
	for (let generation = 0; generation < 2000; generation++) {
		[population, overallStores] = modeFunction[mode](population);
		if (population.length !== populationSize) {
			throw new Error(`Mode ${mode} returned population of size ${population.length} instead of ${populationSize}`);
		}
		stats.x.push(generation);
		stats.y.push(overallStores);
		if (population[ 0 ].stores === 5) {
			console.log("Found solution!");
			break;
		} else {
			console.clear();
			console.log(`Generation ${generation}`);
		}
	}

	console.log(`Best gen: \n Moves: ${population[ 0 ].moves} \n Stores: ${population[ 0 ].stores}`);

	return stats;
}

async function main() {

	// const crossover = run("crossover", rawPopulation);
	// const mutation = run("mutation", rawPopulation);
	// // const eliteCrossover = run("elite-crossover", rawPopulation);
	// // const roulette = run("roulette", rawPopulation);
	// const vadim = run("vadim", rawPopulation);
	const geneticAlgorithm = [];
	for (let i = 0; i < 10; i++) {
		const rawPopulation = genRawPopulation(populationSize);
		const result = run("genetic-algorithm", rawPopulation);
		result.name = `genetic-algorithm-${i}`;
		geneticAlgorithm.push(result);
	}
	const options = { filename: "compare", fileopt: "overwrite" };
	plotly.plot(geneticAlgorithm, options, (err, msg) => { console.log(msg, err); });
}

await main();
