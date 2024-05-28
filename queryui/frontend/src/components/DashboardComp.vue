<template>
  <!-- <WaitModalComp v-if="showModalWait" :message="waitMessage" /> -->
  <div id="wrapper">
    <div id="menu">
      <div class="menu-button-holder">
        <a href="/" id="home"
          ><span><font-awesome-icon icon="fa-solid fa-search" /></span>New Query</a
        >
      </div>
      <canvas id="canvas" style="display: none"></canvas>
    </div>

    <div id="container">
      <div id="queries-wrapper">
        <main>
          <div class="loader">
            <div class="wrapper">
              <div class="circle"></div>
              <div class="circle"></div>
              <div class="circle"></div>
              <div class="shadow"></div>
              <div class="shadow"></div>
              <div class="shadow"></div>
            </div>
          </div>
        </main>
      </div>
      <div id="resizer"><span></span></div>
      <div id="sidebar">
        <section id="sidebar-content">Right</section>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { API } from '@/api'
import { reactive, onMounted } from 'vue'
import { loadState, getElement } from '@/common'

let stateKey = 'query-state'
const state = loadState(stateKey)

const captures: Array<string> = reactive([] as Array<string>)

onMounted(() => {
  const resizer = getElement('#resizer')
  const wrapper = getElement('#queries-wrapper')
  resizer!.addEventListener('mousedown', (event) => {
    document.addEventListener('mousemove', resize, false)
    document.addEventListener(
      'mouseup',
      () => {
        document.removeEventListener('mousemove', resize, false)
      },
      false
    )
  })
  function resize(e: any) {
    const offsetLeft = wrapper!.getBoundingClientRect().left
    const size = `${e.x - offsetLeft}px`
    wrapper!.style.flexBasis = size
  }
  wrapper!.style.flexBasis = '30%'

  console.log('App is ready')
})

function GETCaptures() {
  API.getEndpoints('statistics')!.get('captures')!({}).then(function (response: any) {
    console.log('[Dashboard] Get captures response', response)
    response.data.forEach((base64: { clientId: string; data: string }) => {
      captures.push(base64.data)
    })
  })
}
</script>

<style lang="scss" scoped>
@import '../scss/colors.scss';

#wrapper {
  -webkit-box-orient: vertical;
  -webkit-box-direction: normal;
  height: 100%;
  width: 100%;
  display: grid;
  grid-template-columns: 100px 1fr;
  margin: 0;
  padding: 0;
}

/***************************************************************************************/
/* LEFT MENU */
/***************************************************************************************/
#menu {
  display: grid;
  grid-template-rows: auto auto auto 1fr 1fr auto auto;
  grid-gap: 10px;
  height: 100vh;
  padding: 1rem;
  justify-content: center;
  background-color: rgb(247, 247, 247);
}

#menu h1 {
  text-align: center;
  font-size: 2rem;
}

#menu .menu-button-holder {
  display: grid;
  text-align: center;
  gap: 0;
  font-size: 1.5rem;
}

#menu .menu-button-holder:hover,
#menu .menu-button-holder:hover a {
  cursor: pointer;
  color: $theme-dark-3;
}
#menu .menu-button-holder a {
  display: grid;
  grid-template-rows: auto 1fr;
  font-size: 0.7rem;
  background: none;
  border: none;
  color: $theme-light-1;
  text-decoration: none;
}

#menu .menu-button-holder a span {
  font-size: 1rem;
}
/***************************************************************************************/
/* MIDDLE */
/***************************************************************************************/
#container {
  border-left: 2px white solid;
  width: 100%;
  height: 100%;
  flex-shrink: 0;
  position: relative;
  display: flex;
  overflow: hidden;
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

#container #queries-wrapper {
  position: relative;
  display: grid;
  align-content: center;
  justify-items: center;
  align-items: center;
}
#resizer {
  flex-basis: 2px;
  position: relative;
  z-index: 2;
  cursor: col-resize;
  background: rgb(232, 232, 232);
  margin: 0;
  padding: 0;
  box-sizing: border-box;
  display: table;
}

#resizer:hover {
  background: $blue;
}

#resizer span {
  display: table-cell;
  vertical-align: middle;
  text-align: right;
}

#resizer span:before {
  content: '\2022';
  color: $theme-dark-3;
}

#sidebar {
  flex-basis: 0;
  flex-grow: 1;
  min-width: 0;
  height: 100%;
  flex-direction: row;
  position: relative;
  display: flex;
  margin: 0;
  padding: 0;
  font-size: 0.8rem;
}

#empty {
  height: 100%;
  display: grid;
  text-align: center;
  align-items: center;
  font-size: 1.2rem;
  font-weight: bold;
  background-color: #afafaf;
  color: transparent;
  text-shadow: 1px 1px 1px rgba(255, 255, 255, 0.2);
  -webkit-background-clip: text;
  -moz-background-clip: text;
  background-clip: text;
}

#sidebar-content {
  width: 100%;
  background-color: $theme-light-2;
}

#sidebar-content p {
  width: 100%;
}

#sidebar-content section {
  display: block;
  width: 100%;
  margin-bottom: 1rem;
}

#sidebar-content section#empty-sidebar {
  display: block;
  height: 100%;
}

/***********************************************************************************/
/* LOADER */
/***********************************************************************************/

.loader {
  display: flex;
  align-items: center;
  justify-content: center;
  height: 100%;
  padding: 0;
  margin: 0;
  width: 100%;
}
.loader .wrapper {
  width: 150px;
  height: 60px;
  position: absolute;
  left: 50%;
  transform: translate(-50%, -50%);
}
.loader .circle {
  width: 20px;
  height: 20px;
  position: absolute;
  border-radius: 50%;
  background-color: $theme-dark-3;
  left: 15%;
  transform-origin: 50%;
  animation: circle 0.5s alternate infinite ease;
}

@keyframes circle {
  0% {
    top: 60px;
    height: 5px;
    border-radius: 50px 50px 25px 25px;
    transform: scaleX(1.7);
  }
  40% {
    height: 20px;
    border-radius: 50%;
    transform: scaleX(1);
  }
  100% {
    top: 0%;
  }
}
.circle:nth-child(2) {
  left: 45%;
  animation-delay: 0.2s;
}
.circle:nth-child(3) {
  left: auto;
  right: 15%;
  animation-delay: 0.3s;
}
.shadow {
  width: 20px;
  height: 4px;
  border-radius: 50%;
  background-color: rgba(0, 0, 0, 0.5);
  position: absolute;
  top: 62px;
  transform-origin: 50%;
  z-index: -1;
  left: 15%;
  filter: blur(1px);
  animation: shadow 0.5s alternate infinite ease;
}

@keyframes shadow {
  0% {
    transform: scaleX(1.5);
  }
  40% {
    transform: scaleX(1);
    opacity: 0.7;
  }
  100% {
    transform: scaleX(0.2);
    opacity: 0.4;
  }
}
.shadow:nth-child(4) {
  left: 45%;
  animation-delay: 0.2s;
}
.shadow:nth-child(5) {
  left: auto;
  right: 15%;
  animation-delay: 0.3s;
}
.wrapper span {
  position: absolute;
  top: 75px;
  font-family: 'Lato';
  font-size: 20px;
  letter-spacing: 7px;
  color: $yellow;
  left: 15%;
  color: $gray;
}
</style>
