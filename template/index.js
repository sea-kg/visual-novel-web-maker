window.gameJson = {};

function getJSON(url, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'json';
    xhr.onload = function() {
      var status = xhr.status;
      if (status === 200) {
        window.gameJson = xhr.response;
        callback(null);
      } else {
        callback(status);
      }
    };
    xhr.send();
};

function show_main_progress() {
    document.getElementById("main_loader").style.display = "";
}

function hide_main_progress() {
    document.getElementById("main_loader").style.display = "none";
}

function set_main_progress(new_value) {
    document.getElementById("main_loader_progress").style.width = new_value + "%";
}

function show_main_play_btn() {
    document.getElementById("main_play_btn").style.display = "";
}

function hide_main_play_btn() {
    document.getElementById("main_play_btn").style.display = "none";
}

function hide_screen_loading() {
    document.getElementById("screen_loading").style.display = "none";
}



function loading_resources() {
    console.warn("Simulation loading resources... TODO");
    getJSON("./game.json", function(st, result) {
        if (st !== null) {
            console.error("Could not download ./game.json");
            return;
        } else {
            console.log(result)
            const gameJson = result;
            // const gameJson = JSON.parse(result);
            for (var i in gameJson) {
                gameJson
                console.log(i);
            }
            // TODO parse and
            var progress0 = 0;
            var interval0 = setInterval(function() {
                set_main_progress(progress0);
                progress0 += 10;
                if (progress0 > 100) {
                    clearInterval(interval0);
                    hide_main_progress();
                    show_main_play_btn();
                }
            }, 100);
        }
    })
}

document.addEventListener("DOMContentLoaded", (event) => {
    loading_resources();
});

function run_game() {
    hide_screen_loading();
}