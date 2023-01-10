import { crossoverGens, evaluatePopulation, genRawPopulation, mutateGen } from "./servises.js";
import { createRequire } from "module";
const require = createRequire(import.meta.url);
const plotly = require("plotly")("Mykhailo-Sichkaruk", "gTYne1CKhcfYLhiNfHNe");

const nextGeneration = {
	"mutation": genWithMutation,
	"crossover": genWithCrossover,
	"genetic-algorithm": genWIthGeneticAlgorithm
};

const populationSize = 100;
const maxGeneration = 1000;

function genWithCrossover(population) {
	// Sort population by stores
	const sortedPopulation = population.sort((a, b) => b.stores - a.stores);

	// Get the top 50% of the population
	const topPopulation = sortedPopulation.slice(0, Math.floor(populationSize / 2));
	// Create other 50% of the population by crossover
	let childPopulation = [];
	for (let i = 0; i < Math.floor(populationSize / 2); i++) {
		const parent1 = topPopulation[ i ];
		const parent2 = topPopulation[ Math.floor(Math.random() * Math.floor(populationSize / 2)) ];
		const child = crossoverGens(parent1.gen, parent2.gen);
		childPopulation.push(child);
	}

	// Evaluate the new population
	childPopulation = evaluatePopulation(childPopulation);
	// Merge the top and bottom population
	let nextPopulation = topPopulation.concat(childPopulation);

	// Sort population by stores
	nextPopulation = nextPopulation.sort((a, b) => b.stores - a.stores);

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
	const childrenPopulation = [];
	for (let i = 0; i < Math.floor(populationSize / 2); i++) {
		const parent1 = topPopulation[ i ];
		const parent2 = topPopulation[ Math.floor(Math.random() * Math.floor(populationSize / 2)) ];
		// Crossover parents from top 50% of the population
		const child = crossoverGens(parent1.gen, parent2.gen);
		childrenPopulation.push(child);
	}

	// Mutate childrens of the population
	let childrenMutatedPopulation = [];
	for (const child of childrenPopulation) {
		const gen = mutateGen(child);
		childrenMutatedPopulation.push(gen);
	}

	// Evaluate the new population
	childrenMutatedPopulation = evaluatePopulation(childrenMutatedPopulation);
	// Merge the top and bottom population
	let nextPopulation = topPopulation.concat(childrenMutatedPopulation);

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
	// Create other 50% of the population by mutation
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

function run(mode, rawPopulation) {
	const stats = {
		x: [],
		y: [],
		mode: "markers",
		type: "scatter",
		name: mode,
	};

	// Evaluate given raw population
	let population = evaluatePopulation(rawPopulation);
	// Calculate overall stores of the population
	let overallStores = population.reduce((acc, gen) => acc + gen.stores, 0);
	// Run the cycle maxGeneration times
	for (let generation = 0; generation < maxGeneration; generation++) {
		// Create new generation of population with the corresponding function
		[population, overallStores] = nextGeneration[mode](population);
		// Add the data to the stats
		stats.x.push(generation);
		stats.y.push(overallStores);
		// Check if the best gen has reached the goal
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

async function compare() {
	for (let i = 0; i < 10; i++) {
		const stats = [];

		// Create the initial population
		const rawPopulation = genRawPopulation(populationSize);
		// Run the genetic algorithm
		const geneticAlgorithm = run("genetic-algorithm", rawPopulation);
		// Run only crossover and only mutation
		const crossover = run("crossover", rawPopulation);
		const mutation = run("mutation", rawPopulation);

		// Add the stats to the array
		stats.push(geneticAlgorithm);
		stats.push(crossover);
		stats.push(mutation);

		// Create a graph with the stats
		const options = { filename: `comparation: ${i}`, fileopt: "overwrite" };
		plotly.plot(stats, options, (err, msg) => { console.log(msg, err); });
	}
}

async function testGeneticAlgorithm() {
	const rawPopulation = genRawPopulation(populationSize);

	run("genetic-algorithm", rawPopulation);

}

await testGeneticAlgorithm();

