# SnowInvaders*
<p>A Space Invaders clone made with C and SDL, currently a WIP.</p>
<p>The game is primarily made in C with some C++ features such as namespaces for easier readibility.</p>
<p>*The game currently has an art style similar to the original space invaders but the art will be changed to allow the player to play as jinx against the invasion of the poros. This is so I am able to focus on the game logic first.</p>
<h1>Requirements</h1>
<ul>
  <li>CMAKE</li>
  <li>SDL2</li>
  <li>SDL Image</li>
  <li>SDL Mixer</li>
  <li>SDL TTF</li>
</ul>
<h1>How to run the game</h1>
<bold>Run the following command in the terminal in the root directory to make the build file</bold>

```

cmake -B build

```

<bold>Using the terminal, go into the build folder using the following command:</bold>

```

cd build

```

<bold>Once you are in the build folder, enter the following command to build the program:</bold>

```

make

```

<bold>After the program finishes building, to run the game enter this final command in the terminal:</bold>

```

./SnowInvaders

```

<p>A window should now open with the game.</p>

<h1>Current Game State</h1>

https://github.com/user-attachments/assets/4336ca5a-8d14-410b-9a5a-10101a45e40b

<h2>Completed Tasks</h2>
<ul>
  <li>Rendering and moving player</li>
  <li>Allowing player to shoot</li>
  <li>Rendering and moving aliens/invaders</li>
  <li>Allowing aliens/invaders to be destroyed by player's bullets</li>
</ul>

<h2>Tasks TODO</h2>
<p>These are not in any particular order. They are also subject to change and more features may be added.</p>
<ul>
  <li>Allow aliens/invaders to shoot and destroy the player</li>
  <li>Render barriers and allow them to block bullets</li>
  <li>Make barriers crack when colliding with bullets</li>
  <li>Add HUD to track score and lives</li>
  <li>Reset game after player dies</li>
  <li>Reset aliens after they all die</li>
  <li>Main menus</li>
  <li>Whatever else I feel like adding in the moment... These will be here when I decide to add them to the game.</li>
</ul>
