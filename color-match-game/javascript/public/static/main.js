/*
  Copyright (c) 2015 - 2016 Intel Corporation.

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

$(document).ready(function () {
    var nextLevelFunc = function() {
        $.get('/api/actions/nextLevel');
    }

    $('#btnStartGame').on("click", function () {
        $.get('/api/actions/startGame');
    });

    $('#redButton').on("click", function (data) {
        $.get('/api/colors/red').done(function (data, status, response) {
            $("#colorRedSound")[0].play();
            if (response.status === 204) {
              postponedAlert('nice job...', nextLevelFunc);
            }
        }).fail(function (error) {
          if (error.status === 406) {
            postponedAlert('oops, you made a mistake. click start game to play again.');
          }
        });
    });

    $('#blueButton').on("click", function () {
        $.get('/api/colors/blue').done(function (data, status, response) {
            $("#colorBlueSound")[0].play();
            if (response.status === 204) {
              postponedAlert('nice job...', nextLevelFunc);
            }
        }).fail(function (error) {
          if (error.status === 406) {
            postponedAlert('oops, you made a mistake. click start game to play again.');
          }
        });
    });

    $('#greenButton').on("click", function () {
        $.get('/api/colors/green').done(function (data, status, response) {
            $("#colorGreenSound")[0].play();
            if (response.status === 204) {
              postponedAlert('nice job...', nextLevelFunc);
            }
        }).fail(function (error) {
          if (error.status === 406) {
            postponedAlert('oops, you made a mistake. click start game to play again.');
          }
        });
    });

    $('#yellowButton').on("click", function () {
        $.get('/api/colors/yellow').done(function (data, status, response) {
            $("#colorYellowSound")[0].play();
            if (response.status === 204) {
              postponedAlert('nice job...', nextLevelFunc);
            }
        }).fail(function (error) {
          if (error.status === 406) {
            postponedAlert('oops, you made a mistake. click start game to play again.');
          }
        });
    });

    function postponedAlert(text, funcAfterAlert) {
        setTimeout(function () {
            alert(text);
            if (funcAfterAlert) funcAfterAlert();
        }, 2000);
    }
});
