import{d as _,r as p,A as h,c as e,e as c,f as s,F as u,m as l,_ as m,k as f}from"./index.js";const v={class:"dashboard-component-content"},g={class:"content"},x={class:"Scenary"},b={class:"Conference"},C={class:"Dish"},D=["src"],k=_({__name:"DashboardComp",setup(r){const t=p([]);o();function o(){h.getEndpoints("statistics").get("captures")({}).then(function(a){console.log("[Dashboard] Get captures response",a),a.data.forEach(n=>{t.push(n.data)})})}return(a,n)=>(e(),c("div",v,[s("div",g,[s("div",x,[s("div",b,[s("div",C,[(e(!0),c(u,null,l(t,(d,i)=>(e(),c("div",{key:i,style:{margin:"10px",width:"172px",height:"129px"},"data-aspect":"4:3"},[s("img",{src:d},null,8,D)]))),128))])])])])]))}});const y=m(k,[["__scopeId","data-v-9c064a7c"]]),E=_({__name:"DashboardView",setup(r){return(t,o)=>(e(),f(y))}});export{E as default};
