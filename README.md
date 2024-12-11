
# Catmull-Clark Subdivision Surface


<img src = "images/blub_control_mesh00.png" width = 23%><img src = "images/blub_quadrangulated01.png" width = 25%><img src = "images/blub_quadrangulated02.png" width = 25%><img src = "images/blub_quadrangulated03.png" width = 27%>

<img src = "images/spot_controlmesh00.png" width = 23%><img src = "images/spot_quadrangulated01.png" width = 25%><img src = "images/spot_quadrangulated02.png" width = 25%><img src = "images/spot_quadrangulated03.png" width = 27%>

メッシュの細分割曲面として代表的なCatmull-Clark細分割を研究の一環として実装．  
* 使用言語：C++
* 使用ライブラリ：Eigen
### 細分割曲面
* 細分割曲面を表す制御メッシュを細分割すること得られる曲面
* 繰り返し細分割することで滑らかな曲面を近似するメッシュを得ることができる
* 分割操作を無限回繰り返すことで本来の細分割曲面が表現できる
* 精緻化と平滑化の２段階の処理に分けられる
    - 精緻化：新しい頂点を追加し，面を細分する
    - 平滑化：頂点位置を移動させる

### Catmull-Clark 細分割
多角形メッシュを繰り返し細分割し，四角形メッシュの滑らかな曲面に変換する手法
