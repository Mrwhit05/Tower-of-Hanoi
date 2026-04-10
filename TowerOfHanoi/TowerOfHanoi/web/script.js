Module.onRuntimeIntialized = function () {
    const canvas = document.getElementById("gameCanvas");
    const ctx = canvas.getContext("2d");

    const resetGame = Module.cwrap("resetGame", "void", []);
    const selectPillar = Module.cwrap("selectPillar", "void", ["number"]);
    const placePillar = Module.cwrap("placePillar", "void", ["number"]);
    const isSolved = Module.cwrap("isSolved", "number", []);
    const getMoveCount = Module.cwrap("getMoveCount", "number", []);

    resetGame();

    let hasSelectedDisk = false;

    canvas.addEventListener("click", (e) => {
        const rect = canvas.getBoundingClientRect();
        const x = e.clientX - rect.left;
        const pillar = Math.floor(x / (canvas.width / 3));

        if (!hasSelectedDisk) {
            selectPillar(pillar);
            hasSelectedDisk = true;
        }
        else {
            placePillar(pillar);
            hasSelectedDisk = false;

            if (isSolved()) {
                setTimeout(() => {
                    alert('You win! Solved in ${getMoveCount()} moves');
                    resetGame();
                    hasSelectedDisk = false;
                }, 50);
            }
        }
    });

    function render() {
        ctx.clearRect(0, 0, canvas.width, canvas.height);

        ctx.fillStyle = "white";
        ctx.font = "20px sans-serif";
        ctx.fillText("Moves: " + getMoveCount(), 10, 30);

        ctx.strokeStyle = "white";
        ctx.lineWidth = 4;
        for (let i = 0; i < 3; i++) {
            let x = (canvas.width / 6) + (canvas.width / 3) * i;
            ctx.beginPath();
            ctx.moveTo(x, 450);
            ctx.lineTo(x, 150);
            ctx.stroke();
        }
        //TODO: Draw Disks

        requestAnimationFrame(render);
    }
    render();
}
