/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

WebInspector.NewTabContentView = function(identifier)
{
    var tabBarItem = new WebInspector.TabBarItem("Images/NewTab.svg", WebInspector.UIString("New Tab"));

    WebInspector.TabContentView.call(this, identifier || "new-tab", "new-tab", tabBarItem);

    var allowedNewTabs = [
        {image: "Images/Elements.svg", title: WebInspector.UIString("Elements"), type: WebInspector.ElementsTabContentView.Type},
        {image: "Images/Resources.svg", title: WebInspector.UIString("Resources"), type: WebInspector.ResourcesTabContentView.Type},
        {image: "Images/Storage.svg", title: WebInspector.UIString("Storage"), type: WebInspector.StorageTabContentView.Type},
        {image: "Images/Console.svg", title: WebInspector.UIString("Console"), type: WebInspector.ConsoleTabContentView.Type}
    ];

    for (var info of allowedNewTabs) {
        if (!WebInspector.isTabTypeAllowed(info.type))
            continue;

        var tabItemElement = document.createElement("div");
        tabItemElement.classList.add("tab-item");

        if (WebInspector.isNewTabWithTypeAllowed(info.type))
            tabItemElement.addEventListener("click", this._createNewTab.bind(this, info.type));
        else
            tabItemElement.classList.add("disabled");

        var boxElement = tabItemElement.appendChild(document.createElement("div"));
        boxElement.classList.add("box");

        var imageElement = boxElement.appendChild(document.createElement("img"));
        imageElement.src = info.image;

        var labelElement = tabItemElement.appendChild(document.createElement("label"));
        labelElement.textContent = info.title;

        this.element.appendChild(tabItemElement);
    }
};

WebInspector.NewTabContentView.prototype = {
    constructor: WebInspector.NewTabContentView,
    __proto__: WebInspector.TabContentView.prototype,

    // Public

    get type()
    {
        return WebInspector.NewTabContentView.Type;
    },

    // Private

    _createNewTab: function(tabType, event)
    {
        WebInspector.createNewTab(tabType, this);
    }
};

WebInspector.NewTabContentView.Type = "new-tab";
