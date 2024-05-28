<template>
  <div class="split left">
    <h2>QueryUI</h2>
    <div class="grid-container">
      <div
        v-for="(strategy, index) in strategies"
        v-bind:key="index"
        :class="`show-tooltip button button${index}`"
        :data-tooltip="`${strategy.description}`"
        @click="selectStrategy(strategy)"
      >
        <font-awesome-icon :icon="`fa-solid fa-${strategy.icon}`" />
      </div>
    </div>
  </div>
  <div class="split right">
    <div class="centered">
      <span class="empty" v-if="!selectedStrategy">Click on a Strategy</span>
      <StartComp v-if="selectedStrategy" :strategy="selectedStrategy" />
    </div>
  </div>
</template>

<script setup lang="ts">
import { API } from '@/api'
import { ref, onMounted, reactive } from 'vue'
import StartComp from '@/components/StartComp.vue'

const strategies: Array<any> = reactive([] as Array<any>)
const selectedStrategy = ref(null)

onMounted(() => {
  getStrategies()
})

function getStrategies() {
  API.getEndpoints('strategy')!.get('getAll')!({}).then(function (response: any) {
    console.log('Get all strategies response', response)
    response.data.result.forEach(
      (strategy: {
        name: string
        description: string
        icon: string
        parameters: Array<string>
      }) => {
        strategies.push(strategy)
      }
    )
  })
}

function selectStrategy(strategy: any) {
  selectedStrategy.value = strategy
}
</script>

<style lang="scss" scoped>
@import '../scss/tooltip.scss';

* {
  font-family: 'Product Sans', sans-serif;
}

.split {
  height: 100%;
  width: 50%;
  position: fixed;
  z-index: 1;
  top: 0;
  overflow-x: hidden;
  padding-top: 20px;
}

.left {
  left: 0;
  background-image: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
  background-size: cover;
  background-position: center;
  display: flex;
  justify-content: center;
  align-items: center;
  flex-flow: row wrap;
  flex-direction: column;
}

.left::before {
  content: '';
  background: url('../assets/img/wrapper.png') no-repeat;
  background-size: cover;
  position: absolute;
  top: 0px;
  right: 0px;
  bottom: 0px;
  left: 0px;
  opacity: 0.4;
}

.right {
  right: 0;
  background-color: rgb(241, 241, 241);
}

.centered {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  text-align: center;
}

.centerd img {
  width: 150px;
  border-radius: 50%;
}

h2 {
  font-family: 'Lato', sans-serif;
  background-image: linear-gradient(to right, #feac5e, #c779d0, #4bc0c8);
  -webkit-background-clip: text;
  -moz-background-clip: text;
  background-clip: text;
  color: transparent;
  letter-spacing: 1px;
  font-weight: bold;
  padding: 5px;
  clip-path: inset(2px);
  font-size: 3rem;
  margin-bottom: 2rem;
}

.empty {
  height: 100%;
  display: grid;
  text-align: center;
  align-items: center;
  font-size: 3rem;
  font-weight: bold;
  background-color: #bcbcbc;
  color: transparent;
  text-shadow: 1px 1px 1px rgba(255, 255, 255, 0.1);
  -webkit-background-clip: text;
  -moz-background-clip: text;
  background-clip: text;
}

/***********************************************************************************/
/* STRATEGIES GRID */
/***********************************************************************************/

.grid-container {
  display: grid;
  max-width: 50%;
  grid-template-columns: repeat(auto-fill, 50px);
  align-items: center;
  grid-gap: 5px;
  position: relative;
}

.grid-container .button {
  display: flex;
  justify-content: center;
  align-items: center;
  font-family: 'Open Sans', 'Helvetica Neue', 'Segoe UI', 'Calibri', 'Arial', sans-serif;
  font-size: 18px;
  border: none;
  background: rgb(255, 255, 255);
  background: linear-gradient(180deg, rgba(255, 255, 255, 1) 0%, rgba(251, 251, 251, 1) 100%);
  box-shadow:
    rgba(0, 0, 0, 0.1) 0px 1px 3px 0px,
    rgba(0, 0, 0, 0.06) 0px 1px 2px 0px;
  cursor: pointer;
  transition: all 0.2s;
  border: 1px rgba(255, 255, 255, 0.9) solid;
  border-radius: 0.5rem;
  font-size: 1em;
  padding: 1rem;
}

.grid-container .button:hover {
  transform: scale(0.8);
  background: rgb(250, 250, 250);
  box-shadow:
    rgba(0, 0, 0, 0.14) 0px 1px 3px 0px,
    rgba(0, 0, 0, 0.14) 0px 1px 2px 0px;
}

.grid-container .button span {
  border: 1px red solid;
}

@media screen and (min-width: 37.5em) {
  .grid-container {
    height: auto;
  }
}
</style>
