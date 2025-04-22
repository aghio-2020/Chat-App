

<h1>Build Organization</h1>

- Root folder includes cmake scripts of different components and downloads missing dependencies
- components cmake scripts include all the files and creates the project to then include in the main projects
  - should each component download/include the dependencies they use unless they are common dependencies? Makes more sense