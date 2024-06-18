const NUMBER_PEOPLE = <template>number_people</template>

function themeChange(currentTheme){ //0 par défaut, 0 -> theme clair et 1 -> theme sombre
    let bdheader = document.getElementById("header")
    let dice = document.getElementById('dice').firstElementChild;
    let textFrame = document.getElementById('informations');
    let bdbackground = document.getElementById("body");
    let text = textFrame.querySelectorAll('span');
    let slider = document.getElementById('slider');
    let sun = document.getElementById('sun');
    let moon = document.getElementById('moon');

    if(currentTheme === 'bright'){
        bdheader.style.backgroundColor = '#E8E8E8';
        dice.style.filter = 'brightness(0)';
        textFrame.style.background = '#E0E0E0';
        bdbackground.style.background = 'white';
        text.forEach((elem) => elem.style.color = 'black')
        slider.style.borderColor = '#4B4B4B';
        slider.style.background = '#666666';
        sun.style.backgroundColor = 'rgba(224, 224, 224, 1)';
        moon.style.backgroundColor = 'rgba(102, 102, 102, 0)';
        moon.style.filter = 'drop-shadow(3px 3px 3px #242424)';
        sun.style.filter = "drop-shadow(3px 3px 3px #242424)";
    }

    if(currentTheme === 'dark'){
        bdheader.style.backgroundColor = '#39393A';
        dice.style.filter = 'brightness(100)';
        textFrame.style.background = '#93929C';
        bdbackground.style.background = '#5B5B5B';
        text.forEach((elem) => elem.style.color = 'white');
        slider.style.borderColor = '#A1A1A1';
        slider.style.background = '#E0E0E0';
        sun.style.backgroundColor = 'rgba(224, 224, 224, 0)';
        moon.style.backgroundColor = 'rgba(102, 102, 102, 1)';
        moon.style.filter = 'drop-shadow(0px 0px 0px #242424)';
        sun.style.filter = 'drop-shadow(1px 1px 1px rgba(36, 36, 36, 0.6))';
    }
}


let currentTheme = 'bright';
let slider = document.getElementById('slider');

slider.addEventListener('click', function() {
    if (currentTheme === 'bright'){
        currentTheme = 'dark';
        themeChange(currentTheme);
    } else {
        currentTheme = 'bright';
        themeChange(currentTheme);
    }
});

function goToRandomPage() {
    // Get random person id
    let page_id = Math.floor(Math.random() * (NUMBER_PEOPLE));
    window.location.href = page_id + ".html";
}