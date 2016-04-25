/**
 * Created by staite on 2/9/2016.
 */

$(document).ready(function () {
    $(".audioDemo").trigger('load');

    var nextLevelFunc =function goToNextLevel(){
        $.get('../nextLevel',function(){});
    }
    
    $('#blueButton').click(function () {
        $(".audioDemo").attr('src','simon1.mp3');
        $.get('../blue',function(r){
            $(".audioDemo").trigger('play');
            if(r === 'error'){
                postponedAlert('oops, you made a mistake');
            }
            if(r === 'nextLevel'){
                postponedAlert('nice job...',nextLevelFunc);

            }
        });
    })

    $('#greenButton').click(function () {
        $(".audioDemo").attr('src','simon2.mp3');
        $.get('../green',function(r){
            $(".audioDemo").trigger('play');
            if(r === 'error'){
                postponedAlert('oops, you made a mistake');
            }
            if(r === 'nextLevel'){
                postponedAlert('nice job...',nextLevelFunc);

            }
        });
    })

    $('#redButton').click(function () {
        $(".audioDemo").attr('src','simon3.mp3');
        $.get('../red',function(r){
            $(".audioDemo").trigger('play');
            if(r === 'error'){
                postponedAlert('oops, you made a mistake');
            }
            if(r === 'nextLevel'){
                postponedAlert('nice job...',nextLevelFunc);

            }
        });
    })

    $('#yellowButton').click(function () {
        $(".audioDemo").attr('src','simon4.mp3');
        $.get('../yellow',function(r){
            $(".audioDemo").trigger('play');
            if(r === 'error'){
                postponedAlert('oops, you made a mistake');
            }
            if(r === 'nextLevel'){
                postponedAlert('nice job...',nextLevelFunc);

            }
        });
    })

    $('#btnStartGame').click(function () {
        $.get('../startGame',function(){

        });
    })

    function postponedAlert(text,funcAfterAlert){
        setTimeout(function () {
            alert(text);
            funcAfterAlert();
        },2000);

    }
    

})