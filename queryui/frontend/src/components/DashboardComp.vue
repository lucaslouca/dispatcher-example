<template>
  <div class="dashboard-component-content">
    <div class="content">
      <div class="Scenary">
        <div class="Conference">
          <div class="Dish">
            <div
              v-for="(dataUrl, index) in captures"
              v-bind:key="index"
              style="margin: 10px; width: 172px; height: 129px"
              data-aspect="4:3"
            >
              <img :src="dataUrl" />
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { API } from '@/api'
import { reactive } from 'vue'

const captures: Array<string> = reactive([] as Array<string>)

function GETCaptures() {
  API.getEndpoints('statistics')!.get('captures')!({}).then(function (response: any) {
    console.log('[Dashboard] Get captures response', response)
    response.data.forEach((base64: { clientId: string; data: string }) => {
      captures.push(base64.data)
    })
  })
}
</script>

<style scoped>
.dashboard-component-content {
  width: 100%;
  height: calc(100vh - 75px);
  display: table;
}

.content {
  height: calc(100vh - 75px);
  /* background: rgba(247, 247, 247, 1);
  background: linear-gradient(0deg, rgb(255, 255, 255) 0%, rgba(247, 247, 247, 1) 100%); */
  background-color: white;
}

/***************** */
</style>
