var DATA = [ ];

DATA.push({
  pressure: "30",
  temp: "40",
  altitude: "50",
  sealevel: "60"
});

var screen = new (require("jsupm_i2clcd").SSD1308)(0, 0x3C);

var data = DATA[DATA.length - 1];

screen.clear();

Object.keys(data).forEach(function(name, idx) {
  screen.setCursor(idx, 0);
  screen.write(name + ": " + data[name]);
});

setTimeout(screen.clear.bind(screen), 10 * 1000);
