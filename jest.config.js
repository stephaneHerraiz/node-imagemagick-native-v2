/** @type {import('ts-jest/dist/types').InitialOptionsTsJest} */
module.exports = {
  preset: 'ts-jest',
  testEnvironment: 'node',
  rootDir: "src",
  moduleNameMapper: {
    './Release/imagemagick.node': '../dist/Release/imagemagick.node',
  },
};