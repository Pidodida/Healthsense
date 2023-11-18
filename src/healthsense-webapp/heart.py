import pickle
import streamlit as st
from streamlit_option_menu import option_menu

something_model = pickle.load(open('src/healthsense-webapp/heart.sav', 'rb'))
with st.sidebar:
    
    selected = option_menu('Heart Disease Prediction System', 
                           ['Home'],
                           icons = ['heart'],
                           default_index = 0)

if (selected == 'Home'):
    
    st.title('Heart Disease Prediction using ML')
    
    col1, col2, col3 = st.columns(3)
        
    with col1:
        
        age = st.number_input('Age', min_value=(1), max_value=(100))
        
    with col1:
        
        sex = st.number_input('Sex', min_value=(0), max_value=(1))
    
    with col1:
        
        cp = st.number_input('Chest Pain (0 - 3)', min_value=(0), max_value=(3))
        
    with col2:
        
        fbs = st.number_input('Fasting Blood Sugar > 120mg/dl', min_value=(0), max_value=(1))
        
    with col2:
        
        restecg = st.number_input('Resting ECG (0 - 2)', min_value=(0), max_value=(2))
    
    with col2:
        
        thalach = st.number_input('Maximum Heart rate', min_value=(0), max_value=(202))
        
    with col3:
        
        exang = st.number_input('Exercise induced Angina (0 - 1)', min_value=(0), max_value=(1))

    with col3:
    
        thal = st.number_input('Thalassemia (1 - 3)', min_value=(1), max_value=(3))
    
    with col3:
        
        heart_diagnosis = 'IF THIS TEXT IS SHOWING PLEASE INPUT YOUR DATA'
    
    if st.button('See Results'):
       A = something_model.predict([[age, sex, cp, fbs, restecg, thalach, exang, thal]])
       if (A[0]==0):
            heart_diagnosis = ('I THINK YOU ARE FINE FOR NOW')
       else:
        heart_diagnosis = ('YOU SHOULD SEE A DOCTOR')
    st.success(heart_diagnosis)

