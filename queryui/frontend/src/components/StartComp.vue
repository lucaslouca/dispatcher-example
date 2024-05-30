<template>
  <div class="container">
    <template v-if="strat">
      <h2>{{ strategy.name }}</h2>
      <div class="query-box">
        <span
          v-for="(parameter, index) in strategy.parameters"
          v-bind:key="`row-${index}`"
          class="row"
        >
          <label :for="`input-${parameter}`">{{ parameter }}</label>
          <input
            type="text"
            :id="`input-${parameter}`"
            class="input-field"
            v-model="parameters.SearchFor[parameter]"
            @change="updateUsername"
          />
        </span>

        <button
          class="my-super-cool-btn"
          @click="onStartButtonClick"
          :disabled="username.length == 0"
        >
          <span>Run</span>
        </button>
      </div>
    </template>
  </div>
</template>

<script setup lang="ts">
import { API } from '@/api'
import { ref, onMounted, watch } from 'vue'
import { useRouter } from 'vue-router'
import { loadState, SearchParameters } from '@/common'
import type { State } from '@/common'

const props = defineProps<{
  strategy: object
}>()

const router = useRouter()
let stateKey = 'query-state'
const state = loadState(stateKey)
const username = ref(state.name)

let strat = ref(props.strategy)
let parameters = ref<SearchParameters>(new SearchParameters())

onMounted(() => {})

watch(
  () => props.strategy,
  (newValue, oldValue) => {
    strat.value = newValue
    parameters.value = new SearchParameters()
  }
)

function updateUsername() {
  updateState({ name: username.value })
}

function updateState(newState: Partial<State>): void {
  const state = loadState(stateKey)
  const updatedState = { ...state, ...newState }
  localStorage.setItem(stateKey, JSON.stringify(updatedState))
}

function goToDashboard() {
  router.push({ path: '/dashboard' })
}

function onStartButtonClick() {
  runStrategy({ name: strat.value.name, parameters: parameters })
  // start()
}

function runStrategy(strategy: { name: string; parameters: SearchParameters }) {
  console.log(parameters.value.SearchFor)
  API.getEndpoints('strategy')!.get('create')!({
    name: strategy.name,
    parameters: strategy.parameters.value.SearchFor
  }).then(function (response: any) {
    console.log('Get run strategy response', response)
    if (response.data.success) {
      goToDashboard()
    }
  })
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style lang="scss" scoped>
* {
  font-family: 'Product Sans', sans-serif;
}
h2 {
  font-size: 2.5rem;
  margin-bottom: 2rem;
}
/***********************************************************************************/
/* QUERY INPUT */
/***********************************************************************************/

input.input-field {
  display: block;
  width: 100%;
  margin-bottom: 0.5em;
  box-sizing: border-box;
  padding: 1em;
  border: 3px rgba(255, 255, 255, 0.9) solid;
  border-radius: 300px;
  font-size: 1em;
  background-color: #e3ded9;
  box-shadow: inset 0 0 3px 1px rgba(0, 0, 0, 0.07);
}

input.input-field:focus {
  outline: none;
  background-color: #beb9b3;
  border: 3px #94c2ed solid;
}

.query-box {
  display: grid;
  grid-template-columns: auto;
}

.query-box .row {
  display: grid;
  grid-template-columns: 40% auto;
  align-items: center;
}

.query-box .row label {
  text-transform: capitalize;
  text-align: left;
  padding-right: 1rem;
}

/*********************************************************/
.my-super-cool-btn {
  letter-spacing: 1px;
  font-size: 1rem;
  padding: 1rem;
  width: 100%;
  border: 2px solid $gray;
  border-radius: 300px;
  box-sizing: border-box;
  transition: all 0.5s cubic-bezier(0.25, 1, 0.33, 1);
  box-shadow:
    0 30px 85px rgba(0, 0, 0, 0.14),
    0 15px 35px rgba(0, 0, 0, 0.14);
  background: rgb(255, 255, 255);
  background: linear-gradient(180deg, rgba(255, 255, 255, 1) 0%, rgba(249, 249, 249, 1) 100%);
  cursor: pointer;
  margin-top: 2rem;
}

.my-super-cool-btn span {
  font-family: 'Montserrat', sans-serif;
  background-color: $gray;
  color: transparent;
  text-shadow: 1px 1px 1px rgba(255, 255, 255, 0.3);
  -webkit-background-clip: text;
  -moz-background-clip: text;
  background-clip: text;
  padding: 5px;
  clip-path: inset(2px);
}

.my-super-cool-btn:hover {
  transform: scale(0.95);
  box-shadow:
    0 20px 55px rgba(0, 0, 0, 0.14),
    0 15px 35px rgba(0, 0, 0, 0.14);
}

button:disabled,
button[disabled] {
  background: white;
  border: 1px solid rgb(208, 208, 208);
  color: gray;
  box-shadow: none;
}

button:disabled span,
button[disabled] span {
  background: white;
  color: rgb(234, 234, 234);
  box-shadow: none;
  text-shadow: none;
  -webkit-background-clip: none;
  -moz-background-clip: none;
  background-clip: none;
  padding: 0;
  clip-path: none;
}

button:disabled:hover,
button[disabled]:hover {
  box-shadow: none;
  transform: none;
  cursor: not-allowed;
}
</style>
