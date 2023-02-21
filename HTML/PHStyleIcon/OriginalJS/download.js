function download(selector) {
    // 通过 API 获取目标 canvas 元素
    const canvas = document.querySelector(selector);
    // 创建一个 a 标签，并设置 href 和 download 属性
    const el = document.createElement('a');
    // 设置 href 为图片经过 base64 编码后的字符串，默认为 png 格式
    el.href = canvas.toDataURL();
    el.download = 'Pornhub-sytle.png';
    
    // 创建一个点击事件并对 a 标签进行触发
    const event = new MouseEvent('click');
    el.dispatchEvent(event);
    }