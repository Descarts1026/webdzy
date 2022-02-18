import * as THREE from '../build/three.module.js';
let xGroup = new THREE.Group(),
  helper;
xGroup.name = "xGroup";
let init1 = false;
let modelName = ''
export var distanceShow = function (model,modelName2) {
  modelName = modelName2
  if (!init1) {
    init(model);
    init1 = true;
  }
  // xGroup.visible = true
  model.add(xGroup);
  model.add(helper);
};
export var distanceHide = function (model) {
  model.remove(xGroup);
  // model.remove(helper);
};
function init(model) {
  model.add(xGroup);
  xGroup.visible = true;
  let box3 = new THREE.Box3().expandByObject(model);
  let center = box3.getCenter(new THREE.Vector3());
  let size = box3.getSize(new THREE.Vector3()).length();
  // let center = new THREE.Vector3();
  // box3.getCenter(center);
  let scale = 0.05;
  let ratio = size * scale;
  helper = new THREE.Box3Helper(box3, 0xffff00);
  helper.visible = false;
  model.add(helper);
  console.log(box3);
  const { min, max } = box3;
  console.log(min);
  let zDis = Math.abs(max.z - min.z);
  let xDis = Math.abs(max.x - min.x);
  let yDis = Math.abs(max.y - min.y);
  let xCenter = (max.x + min.x) /2
  let zCenter = (max.z + min.z) /2
  let yCenter = (max.y + min.y) /2
  console.log(xDis, yDis, zDis,xCenter,yCenter);
  let r = 2000;
  let r2 = 1000;
  let x1 = centerModel(model, new THREE.Vector3(max.x, yCenter, max.z));
  let x2 = centerModel(model, new THREE.Vector3(min.x, yCenter, max.z));
  let x3 = centerModel(model, new THREE.Vector3(max.x, yCenter, max.z));
  let x4 = centerModel(
    model,
    new THREE.Vector3(min.x, yCenter, max.z)
  );
  drawLine(xGroup, x1, x2, x3, x4, String(xDis.toFixed(2)) + "m", "x", model);

  let y1 = centerModel(model, new THREE.Vector3(max.x, yDis, zCenter));
  let y2 = centerModel(
    model,
    new THREE.Vector3(max.x, -0,zCenter)
  );
  let y3 = centerModel(model, new THREE.Vector3(max.x, yDis, zCenter ));
  let y4 = centerModel(
    model,
    new THREE.Vector3(max.x, -0,zCenter)
  );
  drawLine(xGroup, y1, y2, y3, y4, String(yDis.toFixed(2)) + "m", "y", model);

  let z1 = centerModel(model, new THREE.Vector3( min.x, yCenter, max.z ));
  let z2 = centerModel(model, new THREE.Vector3( min.x, yCenter, min.z));
  let z3 = centerModel(
    model,
    new THREE.Vector3( min.x, yCenter, max.z)
  );
  let z4 = centerModel(
    model,
    new THREE.Vector3( min.x, yCenter, min.z)
  );
  drawLine(xGroup, z1, z2, z3, z4, String(zDis.toFixed(2)) + "m", "z", model);
  
  let y11 = centerModel(model, new THREE.Vector3(-42333.18779403283,  27785.779754085295, -1999.9999999999854));
  let y21 = centerModel(
    model,
    new THREE.Vector3(-42333.18779403283, 24449.822096293243,-1999.9999999999854)
  );
  let y31 = centerModel(model, new THREE.Vector3(-42333.18779403283,  27785.779754085295, -1999.9999999999854));
  let y41 = centerModel(
    model,
    new THREE.Vector3(-42333.18779403283,24449.822096293243, -1999.9999999999854)
  );
  drawLine(xGroup, y11, y21, y31, y41, String(yDis.toFixed(2)) + "m", "y1", model);
}
function utils_getBox(object1, object2) {
  var matrix = new THREE.Matrix4().compose(
    object2.position,
    object2.quaternion,
    object2.scale
  );
  new THREE.Matrix4().decompose(
    object2.position,
    object2.quaternion,
    object2.scale
  );
  var parent = object2.parent;
  object2.parent = null;
  object2.updateMatrixWorld(true);
  var box = new THREE.Box3().setFromObject(object1);
  object2.parent = parent;
  matrix.decompose(object2.position, object2.quaternion, object2.scale);
  object2.updateMatrixWorld(true);
  return box;
}
// function drawDot($mv, vector) {
//   console.log(vector);
//   let v1 = vector;
//   const geometry = new THREE.SphereGeometry(15000, 32, 16);
//   const material = new THREE.MeshBasicMaterial({ color: 0xffff00 });
//   const sphere = new THREE.Mesh(geometry, material);
//   sphere.position.copy(v1);
//   $mv.model.add(sphere);
// }
function centerModel(model, vector) {
  let box3 = new THREE.Box3().expandByObject(model);
  let center = box3.getCenter(new THREE.Vector3());
  let v1 = new THREE.Vector3().copy(vector);
  return v1
  // return v1.sub(center);
}
function drawLine(
  SizeGroup,
  startPos_f,
  startPos_s,
  endPos_f,
  endPos_s,
  dis,
  normal,
  model
) {
  let t1, t2;
  let mids = new THREE.Vector3(
    (startPos_s.x + endPos_s.x) / 2,
    (startPos_s.y + endPos_s.y) / 2,
    (startPos_s.z + endPos_s.z) / 2
  );
  let midf = new THREE.Vector3(
    (startPos_f.x + endPos_f.x) / 2,
    (startPos_f.y + endPos_f.y) / 2,
    (startPos_f.z + endPos_f.z) / 2
  );
  let box3 = new THREE.Box3().expandByObject(model.clone());
  let size = box3.getSize(new THREE.Vector3()).length();
  let scale = 0.025;
  let ratio = size * scale;
  function SizeLine(startPos_f, startPos_s, endPos_f, endPos_s) {
    var LineGroup = new THREE.Group();
    var directionX = endPos_s.clone().sub(endPos_f.clone());
    var directionX2 = endPos_f.clone().sub(endPos_s.clone());
    var len = 0.0001;
    t1 = directionX.normalize().setLength(len);
    t2 = directionX2.normalize().setLength(len);

    console.log(t1, t2);
    let ends = mids.clone().sub(t1.clone());
    let endf = midf.clone().sub(t2);

    var lengthLine = createLine([
      endf.x,
      endf.y,
      endf.z,
      ends.x,
      ends.y,
      ends.z,
    ]);
    var leftLine = createLine([
      startPos_f.x,
      startPos_f.y,
      startPos_f.z,
      endPos_f.x,
      endPos_f.y,
      endPos_f.z,
    ]);
    var rightLine = createLine([
      startPos_s.x,
      startPos_s.y,
      startPos_s.z,
      endPos_s.x,
      endPos_s.y,
      endPos_s.z,
    ]);
    var leftRowLine = new THREE.ArrowHelper(
      directionX2.normalize(),
      endf,
      len,
      0x00000,
      0.006
    );
    var rightRowLine = new THREE.ArrowHelper(
      directionX.normalize(),
      ends,
      len,
      0x00000,
      0.006
    );
    lengthLine.name = "xGroup";
    leftLine.name = "xGroup";
    rightLine.name = "xGroup";
    leftRowLine.name = "xGroup";
    rightRowLine.name = "xGroup";
    console.log(rightLine);
    LineGroup.add(lengthLine, leftLine, rightLine, leftRowLine, rightRowLine);
    return LineGroup;
    function createLine(pointArr) {
      var material = new THREE.LineBasicMaterial({
        color: 0x00000,
      });
      const lineGeo = new THREE.BufferGeometry();
      const vertices = new Float32Array(pointArr);
      lineGeo.setAttribute("position", new THREE.BufferAttribute(vertices, 3));
      var line = new THREE.Line(lineGeo, material);
      return line;
    }
  }
  var SizeLineX = SizeLine(startPos_f, startPos_s, endPos_f, endPos_s);
  SizeLineX.name = "xGroup";
  SizeGroup.add(SizeLineX);
  var p2 = new THREE.Vector3(
    (endPos_s.x + endPos_f.x) / 2,
    (endPos_s.y + endPos_f.y) / 2,
    (endPos_s.z + endPos_f.z) / 2
  );
  new THREE.FontLoader().load("./FZShuSong-Z01S_Regular.json", (font) => {
    if(modelName == '馨园'){
      if(normal == 'y'){
        dis = '15.212m'
      }
      if(normal == 'x'){
        dis = '36.456m'
      }
      if(normal == 'z'){
        dis = '20.626m'
      }
      if(normal == 'y1'){
        dis = '2.5m'
        ratio = ratio * 0.5
      }
    }
    if(modelName == '知行楼'){
      if(normal == 'y'){
        dis = '36.538m'
      }
      if(normal == 'x'){
        dis = '164.578m'
      }
      if(normal == 'z'){
        dis = '110.341m'
      }
      if(normal == 'y1'){
        dis = '2.5m'
        ratio = ratio * 0.5
      }
    }
    const textGeom = new THREE.TextGeometry(dis, {
      font: font,
      size: ratio,
      height: 0,
      // curveSegments: 1,
      // bevelEnabled: false,
      // bevelThickness: 0,
      // bevelSize: 0,
      // bevelSegments: 1
    });
    const textMat = new THREE.MeshBasicMaterial({
      color: 0x00000,
    });
    const obj = new THREE.Mesh(textGeom, textMat);
    console.log(textGeom, obj);
    let cM, nM, normall;
    if (normal == "x") {
      cM = new THREE.Vector3(0, 0, -1);
      nM = new THREE.Vector3(0, 1, 0);
      normall = new THREE.Vector3(0, 1, 0);
    }
    if (normal == "y" || normal == 'y1') {
      cM = new THREE.Vector3(1, 0, 0);
      nM = new THREE.Vector3(0, 0, 1);
      normall = new THREE.Vector3(0, 1, 0);
      obj.rotateOnAxis(new THREE.Vector3(0, 0, 1), (Math.PI / 2) * 3);
    }
    if (normal == "z") {
      cM = new THREE.Vector3(0, -1, 0);
      nM = new THREE.Vector3(0, 0, 1);
      normall = new THREE.Vector3(0, 0, 1);
      obj.rotateOnAxis(new THREE.Vector3(0, 0, 1), (Math.PI / 2) * 3);
    }
    obj.rotateOnAxis(normall, Math.PI);
    let quent = new THREE.Quaternion();
    quent.setFromUnitVectors(cM, nM);
    obj.applyQuaternion(quent);
    // obj.rotateOnAxis(normal, Math.PI/2)
    // obj.rotateOnAxis(new THREE.Vector3(0, 1, 0), Math.PI)
    obj.position.copy(p2);
    SizeGroup.add(obj);
    obj.name = "xGroup";
  });
  // SizeGroup.add(sprite);
}
