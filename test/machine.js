import { describe, it }  from "node:test";
import { createRequire } from "module";
import assert from "assert";
const require = createRequire(import.meta.url);
const Machine = require("../build/Release/machine");

const wrongMachineConstructorArgs = [
	[ 123 ],
	[ new ArrayBuffer(64), 10],
	[ "ASDF" ],
	[ true ],
	[ {} ],
	[],
];

describe("Machine", () => {
	describe("constructor", () => {
		it("should throw an error if first argument is not an ArrayBuffer", () => {
			wrongMachineConstructorArgs.forEach(args => {
				assert.throws(() => { new Machine.Machine(...args); },
					TypeError);
			});
		});
	});
});
