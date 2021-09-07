var a;
var b;
var n;
var e;
var x;
var mass;
var load;
var c;
function submitValues() {
  document.getElementById('solution').innerHTML = "<h3>Output</h3>";
  initializeValues();
  assembleMassMatrix();
  assembleLoadVector();
  augmentMass();
  console.log(mass);
  rref();
  for(var i = 0; i < n; i++) {
     console.log(mass[i][n]);
    c[i] = math.fraction(`${mass[i][n].n*mass[i][n].s}/${mass[i][n].d}`);
  }
  // output(get_equations());
  output(getPhU());
}

function initializeValues() {
  a = math.fraction(document.getElementById('a').value);
  b = math.fraction(document.getElementById('b').value);
  n = parseInt(document.getElementById('nodes').value);
  e = document.getElementById('expression').value;
  getSubintervals();
  c = [];
}

function getSubintervals() {
  x = [];
  if(!document.getElementById('even').checked) {
     var manual = document.getElementById('intervals').value.split(',');
     //output("x = ");
     for(var i = 0; i < manual.length; i++) {
       x[i] = math.fraction(manual[i]);
       // console.log(x[i]);
       //output("| " + x[i] + " |");
     }
    return;
  }
  var h = math.fraction(`${b.sub(a).n*b.sub(a).s}/${b.sub(a).d}`);
  h = h.div(math.fraction(`${n - 1}`));
  // output("x = ");
  for(var i = 0; i < n; i++) {
    x[i] = a.add(h.mul(i));
    // console.log(x[i]);
    //output("| " + x[i] + " |");
  }
}

function output(str) {
  document.getElementById('solution').innerHTML += "<h5>" + str + "</h5>";
}


// ------ L-2 Projection Methods ------ //

function assembleMassMatrix() {
  //initialize 2-D array n x n with additional column for load vector.
  mass = new Array(n).fill(math.fraction('0')).map(x => new Array(n+1).fill(math.fraction('0')));
  for(var i = 0; i < n-1; i++) {
    // console.log(x[i+1].sub(x[i]));
    var hi6 = (x[i+1].sub(x[i])).div(math.fraction('6'));
    var mi = [[hi6.mul(math.fraction('2')),hi6],[hi6,hi6.mul(math.fraction('2'))]];
    mass[i][i] = (mass[i][i]).add(mi[0][0]);
    mass[i][i+1] = (mass[i][i+1]).add(mi[0][1]);
    mass[i+1][i] = (mass[i+1][i]).add(mi[1][0]);
    mass[i+1][i+1] = (mass[i+1][i+1]).add(mi[1][1]);
  }
}

function assembleLoadVector() {
  load = new Array(n).fill(math.fraction('0'));
  for(var i = 0; i < n-1; i++) {
    var hi2 = (x[i+1].sub(x[i])).div(math.fraction('2'));
    var bi = [hi2.mul(getFunctionVal(x[i])),hi2.mul(getFunctionVal(x[i+1]))];
    load[i] = (load[i]).add(bi[0]);
    load[i+1] = (load[i+1]).add(bi[1]);
  }
}

// function get_equations() {
//   var a = "f_{0}(x)=" + get_equation_zero(x[0],x[1],x[n-1]);
// 	for (var i = 1; i < n-1; i++) {
// 		a += "<br/><br/>" + "f_{"+i+"}(x)="+get_equation(x[0],x[i-1],x[i],x[i+1],x[n-1]);
//   }
//   var n1 = n-1;
// 	a += "<br/><br/>" + "f_{"+n1+"}(x)="+get_equation_n(x[0],x[n-2],x[n-1]);
// 	return a;
// }

// function get_equation_zero(x0, x1, xn){
// 	var diff = x1.sub(x0);
// 	return `{0<=x<=${x1.n*x1.s}/${x1.d}:(${x1.n*x1.s}/${x1.d}-x)/(${diff.n*diff.s}/${diff.d}),${x1.n*x1.s}/${x1.d}<=x<=${xn.n*xn.s}/${xn.d}:0}`;
// }
//
// function get_equation_n(x0,xn1, xn){
// 	var diff = xn.sub(xn1);
// 	return `{${xn1.n*xn1.s}/${xn1.d}<=x<=${xn.n*xn.s}/${xn.d}:(x-${xn1.n*xn1.s}/${xn1.d})/(${diff.n*diff.s}/${diff.d}),${x0.n*x0.s}/${x0.d}<=x<=${xn1.n*xn1.s}/${xn1.d}:0}`;
// }
//   function get_equation(x0,xi1, xi, xi2,xn){
//   	var diff1 = xi.sub(xi1);
//   	var diff2 = xi2.sub(xi);
//   	return `{${x0.n*x0.s}/${x0.d}<=x<=${xi1.n*xi1.s}/${xi1.d}:0,${xi1.n*xi1.s}/${xi1.d}<=x<=${xi.n*xi.s}/${xi.d}:(x-${xi1.n*xi1.s}/${xi1.d})/(${diff1.n*diff1.s}/${diff1.d}),${xi.n*xi.s}/${xi.d}<=x<=${xi2.n*xi2.s}/${xi2.d}:(${xi2.n*xi2.s}/${xi2.d}-x)/${diff2.n*diff2.s}/${diff2.d},${xi2.n*xi2.s}/${xi2.d}<=x<=${xn.n*xn.s}/${xn.d}:0}`;
// }
function getPhU() {
  var mystring = `y = {${x[0].n*x[0].s}/${x[0].d}<=x<=${x[1].n*x[1].s}/${x[1].d}:${((c[1].sub(c[0])).div(x[1].sub(x[0]))).n*((c[1].sub(c[0])).div(x[1].sub(x[0]))).s}/${((c[1].sub(c[0])).div(x[1].sub(x[0]))).d}(x-${x[0].n*x[0].s}/${x[0].d})+${c[0].n*c[0].s}/${c[0].d}`;
  for(var i = 2; i < n; i++) {
    var x1 = x[i-1];
    var x2 = x[i];
    var y1 = c[i-1];
    var y2 = c[i];
    var slope = (y2.sub(y1)).div(x2.sub(x1));
    mystring += `,${x1.n*x1.s}/${x1.d}<=x<=${x2.n*x2.s}/${x2.d}:${slope.n*slope.s}/${slope.d}(x-${x1.n*x1.s}/${x1.d})+${y1.n*y1.s}/${y1.d}`;
  }
  mystring += "}";
  return mystring;
}

function augmentMass() {
  for(var i = 0; i < n; i++) {
    //console.log(`${load[i].n}/${load[i].d}`);
    mass[i][n] = math.fraction(`${load[i].n*load[i].s}/${load[i].d}`);
  }
}

function getFunctionVal(x) {
  let scope = {
    x: x
  }
  return math.evaluate(e, scope);
  // var ret = ""+Math.atan((x - 0.5)/0.01);
  // var count = Math.pow(10, ret.substring(ret.indexOf('.'), ret.length).length-1);
  // ret = ret.replace('.','') + "/" + count;
  // console.log(ret);
  // return math.fraction(ret);
}

function rref() {
  ref();
  for(var i = n-2; i>= 0; i--) {
    subtractRowByFactor(mass[i], mass[i+1],mass[i][i+1]);
  }
}

function ref() {
  for(var i = 0; i < n; i++) {
    for(var j = 0; j < i; j++) {
      subtractRowByFactor(mass[i], mass[j], mass[i][j]);
    }
    multiplyRowBy(mass[i], math.fraction(`${mass[i][i].d}/${mass[i][i].n*mass[i][i].s}`));
  }
}

function multiplyRowBy(row, factor) {
  for(var i = 0; i < n+1; i++) {
    row[i] = row[i].mul(factor);
  }
}

function subtractRowByFactor(row1, row2, factor) {
  for(var i = 0; i <= n; i++) {
    row1[i] = row1[i].sub(factor.mul(row2[i]));
  }
}
