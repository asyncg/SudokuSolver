const grid = document.getElementById("grid");

for (let i = 0; i < 81; i++) {
  const input = document.createElement("input");
  input.type = "tel";
  input.inputMode = "numeric";
  input.maxLength = 1;
  input.dataset.index = i;
  grid.appendChild(input);
}

const inputs = Array.from(document.querySelectorAll("#grid input"));

inputs.forEach((input, i) => {
  input.addEventListener("focus", () => hl(i));
  input.addEventListener("blur", unhl);
});

function hl(index) {
  const row = Math.floor(index / 9);
  const col = index % 9;
  const boxRow = Math.floor(row / 3) * 3;
  const boxCol = Math.floor(col / 3) * 3;

  inputs.forEach((input, i) => {
    const r = Math.floor(i / 9);
    const c = i % 9;
    if (r === row) input.classList.add("highlight-row");
    if (c === col) input.classList.add("highlight-col");
    if (r >= boxRow && r < boxRow + 3 && c >= boxCol && c < boxCol + 3) {
      input.classList.add("highlight-box");
    }
  });
}

function unhl() {
  inputs.forEach((input) =>
    input.classList.remove("highlight-row", "highlight-col", "highlight-box"),
  );
}

function wipe() {
  inputs.forEach((input) => {
    input.value = "";
    input.disabled = false;
    input.classList.remove("prefilled");
  });
}

function getPtr() {
  const board = new Int32Array(81);
  inputs.forEach((input, i) => {
    const val = parseInt(input.value);
    board[i] = isNaN(val) ? 0 : val;
  });

  const ptr = Module._malloc(81 * 4);
  Module.HEAP32.set(board, ptr / 4);
  return ptr;
}

function setGrid(ptr, lock = false) {
  const board = Module.HEAP32.subarray(ptr / 4, ptr / 4 + 81);
  board.forEach((val, i) => {
    const input = inputs[i];
    input.value = val ? val : "";
    if (lock && val) {
      input.disabled = true;
      input.classList.add("prefilled");
    }
  });
}

function check(ptr) {
  return Module.ccall("validate_board", "boolean", ["number"], [ptr]);
}

function gen() {
  wipe();

  const ptr = Module._malloc(81 * 4);
  Module.HEAP32.fill(0, ptr / 4, ptr / 4 + 81);

  Module._generate_board(ptr);
  setGrid(ptr, true);

  Module._free(ptr);
}

function solve() {
  const ptr = getPtr();

  if (!check(ptr)) {
    alert("Invalid board. Fix mistakes first.");
    Module._free(ptr);
    return;
  }

  inputs.forEach((input) => {
    const val = parseInt(input.value);
    if (!isNaN(val) && val > 0) {
      input.disabled = true;
      input.classList.add("prefilled");
    }
  });

  const ok = Module.ccall("solve_board", "boolean", ["number"], [ptr]);

  if (ok) {
    setGrid(ptr);
  } else {
    alert("This puzzle could not be solved.");
  }

  Module._free(ptr);
}

document.getElementById("generate").addEventListener("click", gen);
document.getElementById("solve").addEventListener("click", solve);
document.getElementById("clear").addEventListener("click", () => {
  inputs.forEach((input) => {
    if (!input.classList.contains("prefilled")) input.value = "";
  });
});
document.getElementById("full-clear").addEventListener("click", wipe);
