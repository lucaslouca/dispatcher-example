import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

const path = require('path')

// https://vitejs.dev/config/
export default defineConfig(({ mode }) => ({
  plugins: [vue()],
  base: mode === 'production' ? '/' : '/',
  build: {
    outDir: '../backend/ui/',
    assetsDir: '',
    rollupOptions: {
      output: {
        entryFileNames: `[name].js`,
        chunkFileNames: `[name].js`,
        assetFileNames: `[name].[ext]`
      }
    }
  },
  resolve: {
    alias: [{ find: '@', replacement: path.resolve(__dirname, '/src') }]
  },
  css: {
    preprocessorOptions: {
      scss: {
        additionalData: `
          @use "@/scss/colors" as *;
        `,
        api: 'modern-compiler'
      }
    }
  }
}))
