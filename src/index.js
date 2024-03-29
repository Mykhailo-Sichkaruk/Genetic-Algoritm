import { createRequire } from "module";
const require = createRequire(import.meta.url);
const Machine = require("../build/Release/machine");
const Game = require("../build/Release/game");

const MEMORY_SIZE = 64;

const generateArrayBufferMemory = size => {
	const memory = new ArrayBuffer(size);
	const memoryView = new Uint8Array(memory);
	for (let i = 0; i < size; i++) {
		memoryView[ i ] = Math.floor(Math.random() * 255);
	}

	return memory;
};

const memory = generateArrayBufferMemory(MEMORY_SIZE);
console.log("Memory: ", memory);
console.log("Running machine...");
const result = Machine.run(memory, MEMORY_SIZE);
console.log("Result:", result);

console.log("Running game...");
const stores = Game.run(result);
console.log("Stores:", stores);
