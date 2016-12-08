/**
 * Created by staite on 2/9/2016.
 */

$(document).ready(function () {

    var nextLevelFunc = function() {
        $.get('/api/actions/nextLevel');
    }

    $('#btnStartGame').on("click", function () {
        $.get('/api/actions/startGame');
    })

    $('#redButton').on("click", function () {
        $.get('/api/colors/red').done(function (data, status, response) {
            $("#colorRedSound")[0].play();
            if (response.status === 406) {
                postponedAlert('oops, you made a mistake');
            }
            if (response.status === 204) {
                postponedAlert('nice job...', nextLevelFunc);
            }
        });
    })

    $('#blueButton').on("click", function () {
        $.get('/api/colors/blue').done(function (data, status, response) {
            $("#colorBlueSound")[0].play();
            if (response.status === 406) {
                postponedAlert('oops, you made a mistake');
            }
            if (response.status === 204) {
                postponedAlert('nice job...', nextLevelFunc);
            }
        });
    })

    $('#greenButton').on("click", function () {
        $.get('/api/colors/green').done(function (data, status, response) {
            $("#colorGreenSound")[0].play();
            if (response.status === 406) {
                postponedAlert('oops, you made a mistake');
            }
            if (response.status === 204) {
                postponedAlert('nice job...', nextLevelFunc);
            }
        });
    })

    $('#yellowButton').on("click", function () {
        $.get('/api/colors/yellow').done(function (data, status, response) {
            $("#colorYellowSound")[0].play();
            if (response.status === 406) {
                postponedAlert('oops, you made a mistake');
            }
            if (response.status === 204) {
                postponedAlert('nice job...', nextLevelFunc);
            }
        });
    })

    function postponedAlert(text, funcAfterAlert) {
        setTimeout(function () {
            alert(text);
            funcAfterAlert();
        }, 2000);

    }
})