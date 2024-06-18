const NUMBER_PEOPLE = <template>number_people</template>
const ID_ARRAY = <template>array_ids</template>

function themeChange(currentTheme){ //0 par défaut, 0 -> theme clair et 1 -> theme sombre
    let bdheader = document.getElementById("header");
    let dice = document.getElementById('dice').firstElementChild;
    let bdbackground = document.getElementById("body");
    let slider = document.getElementById('slider');
    let sun = document.getElementById('sun');
    let moon = document.getElementById('moon');
    let borders = document.querySelectorAll('.actual-member, .mama, .papa, .gp-papa1, .gp-papa2, .gp-mama2, .gp-mama1');
    let photoBorders = document.querySelectorAll('.actual-member img, .photo-mam, .photo-pap, .photo-gpap1, .photo-gpap2, .photo-gmam1, .photo-gmam2')
    let texts = document.querySelectorAll('span')

    if(currentTheme === 'bright'){
        bdheader.style.backgroundColor = '#E8E8E8';
        dice.style.filter = 'brightness(0)';
        bdbackground.style.background = 'white';
        slider.style.borderColor = '#4B4B4B';
        slider.style.background = '#666666';
        sun.style.backgroundColor = 'rgba(224, 224, 224, 1)';
        moon.style.backgroundColor = 'rgba(102, 102, 102, 0)';
        moon.style.filter = 'drop-shadow(3px 3px 3px #242424)';
        sun.style.filter = "drop-shadow(3px 3px 3px #242424)";
        borders.forEach((elem) => elem.style.borderColor = 'black');
        texts.forEach((elem) => elem.style.color = 'black');
        photoBorders.forEach((elem) => elem.style.borderColor = 'black');
    }

    if(currentTheme === 'dark'){
        bdheader.style.backgroundColor = '#39393A';
        dice.style.filter = 'brightness(100)';
        bdbackground.style.background = '#5B5B5B';
        slider.style.borderColor = '#A1A1A1';
        slider.style.background = '#E0E0E0';
        sun.style.backgroundColor = 'rgba(224, 224, 224, 0)';
        moon.style.backgroundColor = 'rgba(102, 102, 102, 1)';
        moon.style.filter = 'drop-shadow(0px 0px 0px #242424)';
        sun.style.filter = 'drop-shadow(1px 1px 1px rgba(36, 36, 36, 0.6))';
        borders.forEach((elem) => elem.style.borderColor = '#9e9d9d');
        texts.forEach((elem) => elem.style.color = 'white');
        photoBorders.forEach((elem) => elem.style.borderColor = 'white');
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

function goToPage(newPage) {
    window.location.href = newPage;
}

function goToRandomPage() {
    // Get random person id
    let page_id = Math.floor(Math.random() * (NUMBER_PEOPLE));
    window.location.href = page_id + ".html";
}