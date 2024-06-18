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
    let page_id = Math.floor(Math.random() * (ID_ARRAY.length));
    window.location.href = ID_ARRAY[page_id] + ".html";
}

function createCanvas() {
    let canvas = document.createElement('canvas');
    canvas.style.position = 'absolute';
    canvas.style.top = '0';
    canvas.style.left = '0';
    canvas.style.width = '100%';
    canvas.style.height = '100%';
    canvas.style.pointerEvents = 'none';
    document.body.appendChild(canvas);
    return canvas;
}


function resizeCanvas(canvas) {
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
}


function drawArrow(context, fromx, fromy, tox, toy){
    let headlen = 10; // longueur de la tête en pixels
    let dx = tox - fromx;
    let dy = toy - fromy;
    let angle = Math.atan2(dy, dx);
    context.beginPath();
    context.moveTo(fromx, fromy);
    context.lineTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
    context.stroke();
}


function positionArrow(element1, element2, context, canvas) {
    resizeCanvas(canvas);
    let rect1 = element1.getBoundingClientRect();
    let rect2 = element2.getBoundingClientRect();
    let x1 = rect1.right; // bord droit de l'élément 1
    let y1 = rect1.top + rect1.height / 2; // centre vertical de l'élément 1
    let x2 = rect2.left; // bord gauche de l'élément 2
    let y2 = rect2.top + rect2.height / 2; // centre vertical de l'élément 2
    context.strokeStyle = 'red';
    context.lineWidth = 2;
    drawArrow(context, x1, y1, x2, y2);
}


function main() {
    let mama = document.getElementById('mama');
    let member = document.getElementById('member');
    let papa = document.getElementById('papa')
    let gmama=document.getElementById('gmama')
    let gmamabis = document.getElementById('gmamabis')

    let canvas1 = createCanvas();
    let context1 = canvas.getContext('2d');
    let canvas2 = createCanvas();
    let context2 = canvas.getContext('2d');
    let canvas3 = createCanvas();
    let context3 = canvas.getContext('2d');
    let canvas4 = createCanvas();
    let context4 = canvas.getContext('2d');
    
    positionArrow(mama, member, context1, canvas1);
    window.addEventListener('resize', () => positionArrow(mama, member, context1, canvas1));
    positionArrow(papa, member, context2, canvas2);
    window.addEventListener('resize', () => positionArrow(papa, member, context2, canvas2));
    positionArrow(gmama, mama, context3, canvas3);
    window.addEventListener('resize', () => positionArrow(gmama, mama, context3, canvas3));
    positionArrow(gmamabis, papa, context4, canvas4);
    window.addEventListener('resize', () => positionArrow(gmamabis, papa, context4, canvas4));
}

main();
